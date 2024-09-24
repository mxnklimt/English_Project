#define W25Q32_C_

#include "w25q32.h"

#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

//初始化SPI1 FLASH的IO口
void SPI_W25Q32_Init(void)
{ 
	GPIOA_SetBits( GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6 );
	GPIOA_ModeCfg( GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6, GPIO_ModeOut_PP_5mA );	// MOSI/SCK/CS
	GPIOA_SetBits( GPIO_Pin_2 );
	GPIOA_ModeCfg( GPIO_Pin_2, GPIO_ModeIN_PU );	// MISO	
	
  SPI1_MasterDefInit();
  SPI1_DataMode( Mode3_HighBitINFront );
  SPI1_CLKCfg( 4 );
  debug_printf("id:0x%04x\r\n", W25Q32_ReadID() );//读取FLASH ID.
	
		
}  

//读取芯片ID		     
uint16_t W25Q32_ReadID(void)
{
	uint16_t Temp = 0;	  
	SPI1_CS_LOW();				    
  SPI1_MasterSendByte(CMD_DEVICE_ID);                    //读取ID命令
  SPI1_MasterSendByte(0x00);
  SPI1_MasterSendByte(0x00);
  SPI1_MasterSendByte(0x00);	
	
	Temp|=SPI1_MasterRecvByte()<<8;  
	Temp|=SPI1_MasterRecvByte();	 
  SPI1_CS_HIGH();				    
	return Temp;
}  

//读取W25QXX的状态寄存器
//默认:0x00
uint8_t W25Q32_ReadSR(void)   
{  
	uint8_t byte=0;  
	
	SPI1_CS_LOW();                             //使能器件   
	SPI1_MasterSendByte(CMD_STATUS1);    //发送读取状态寄存器命令    
	byte=SPI1_MasterRecvByte();             //读取一个字节  
  SPI1_CS_HIGH();                           //取消片选     
	return byte;   
} 


//写W25QXX状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void W25Q32_Write_SR(uint8_t sr)   
{   
	SPI1_CS_LOW();                            //使能器件   
	SPI1_MasterSendByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令    
	SPI1_MasterSendByte(sr);               //写入一个字节  
  SPI1_CS_HIGH();                             //取消片选     	      
}  

//W25QXX写使能	
//将WEL置位   
void W25Q32_Write_Enable(void)   
{
	SPI1_CS_LOW();                            //使能器件   
  SPI1_MasterSendByte(W25X_WriteEnable);      //发送写使能  
  SPI1_CS_HIGH();                             //取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
void W25Q32_Write_Disable(void)   
{  
	SPI1_CS_LOW();                           //使能器件   
  SPI1_MasterSendByte(W25X_WriteDisable);     //发送写禁止指令    
  SPI1_CS_HIGH();                            //取消片选     	      
} 

//读取SPI FLASH  
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void W25Q32_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	  uint16_t i;   										    
	  SPI1_CS_LOW();                              //使能器件   
    SPI1_MasterSendByte(W25X_ReadData);         //发送读取命令   
    SPI1_MasterSendByte((uint8_t)((ReadAddr)>>16));  //发送24bit地址    
    SPI1_MasterSendByte((uint8_t)((ReadAddr)>>8));   
    SPI1_MasterSendByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	   { 
        pBuffer[i]=SPI1_MasterRecvByte();   //循环读数  
     }
	  SPI1_CS_HIGH();  				    	      
} 

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
void W25Q32_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25Q32_Write_Enable();                  //SET WEL 
	SPI1_CS_LOW();                            //使能器件   
  SPI1_MasterSendByte(W25X_PageProgram);      //发送写页命令   
  SPI1_MasterSendByte((uint8_t)((WriteAddr)>>16)); //发送24bit地址    
  SPI1_MasterSendByte((uint8_t)((WriteAddr)>>8));   
  SPI1_MasterSendByte((uint8_t)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)SPI1_MasterSendByte(pBuffer[i]);//循环写数  
	SPI1_CS_HIGH();                            //取消片选 
	W25Q32_Wait_Busy();					   //等待写入结束
} 

//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25Q32_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q32_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
uint8_t W25Q32_BUFFER[4096];		 
void W25Q32_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25Q32_BUF;	  
   	W25Q32_BUF=W25Q32_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址  
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q32_Read(W25Q32_BUF,secpos*4096,4096);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25Q32_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25Q32_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25Q32_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q32_Write_NoCheck(W25Q32_BUF,secpos*4096,4096);//写入整个扇区  

		}else W25Q32_Write_NoCheck(pBuffer,WriteAddr,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			WriteAddr+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096)secremain=4096;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}
/*
*********************************************************************************************************
*	函 数 名: sf_CmpData
*	功能说明: 比较Flash的数据。
*	形    参:  	_ucpTar : 数据缓冲区。
*				_uiSrcAddr ：Flash地址。
*				_uiSize ：数据个数, 不能超出芯片总容量。
*	返 回 值: 0 = 相等, 1 = 不等
*********************************************************************************************************
*/
uint8_t W25Q32_CmpData(uint8_t* pBuffer,uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint16_t i;


	SPI1_CS_LOW(); 									/* 使能片选 */
  SPI1_MasterSendByte(W25X_ReadData);         //发送读取命令   
  SPI1_MasterSendByte((uint8_t)((_uiSrcAddr)>>16));  //发送24bit地址    
  SPI1_MasterSendByte((uint8_t)((_uiSrcAddr)>>8));   
  SPI1_MasterSendByte((uint8_t)_uiSrcAddr);   
  for(i=0;i<_uiSize;i++)
	 { 
     pBuffer[i]=SPI1_MasterRecvByte();   //循环读数  
		 if(pBuffer[i] != *_ucpTar++)
			{
			  debug_printf("W25Q32_CmpData i=%02d: len=%02d: ",i ,*_ucpTar);
				goto NOTEQ;		       /* 不相等 */
			}
   }

  SPI1_CS_HIGH();
	return 0;		/* 相等 */
	
NOTEQ:	
	SPI1_CS_HIGH();	/* 不相等 */
	return 1;
}

/*
*********************************************************************************************************
*	函 数 名: sf_CmpData
*	功能说明: 比较Flash的数据。
*	形    参:  	_ucpTar : 数据缓冲区。
*				_uiSrcAddr ：Flash地址。
*				_uiSize ：数据个数, 不能超出芯片总容量。
*	返 回 值: 0 = 相等, 1 = 不等
*********************************************************************************************************
*/
uint8_t W25Q32_CmpMacData(uint32_t _uiSrcAddr, uint8_t *_ucpTar,uint32_t _uiSize)
{
	uint16_t i , j ;
  uint8_t pBuffer[6] = {0};
  uint32_t uiSrcAddr = _uiSrcAddr ;
 
  for(i=0;i<_uiSize;i=i+6)
	 { 
		 SPI1_CS_LOW();						                  /* 使能片选 */
     SPI1_MasterSendByte(W25X_ReadData);                //发送读取命令   
     SPI1_MasterSendByte((uint8_t)((uiSrcAddr)>>16));  //发送24bit地址    
     SPI1_MasterSendByte((uint8_t)((uiSrcAddr)>>8));   
     SPI1_MasterSendByte((uint8_t)uiSrcAddr);		 
     debug_printf("W25Q32_CmpData55555 len=%02d: \r\n", uiSrcAddr); 		 
	   for(j=0;j<6;j++)	 
	    {
				pBuffer[j]=SPI1_MasterRecvByte();   //循环读数  	
        if(pBuffer[j] != _ucpTar[j])
			   {	  
				    goto NOTEQ;		       /* 不相等 */
			   }				
		  }
			SPI1_CS_HIGH();
			return i;                   /* 相等 */
			NOTEQ:
			SPI1_CS_HIGH();	/* 不相等 */
      debug_printf("W25Q32_CmpData55555 len=%02d: \r\n", 6);
      log_hex((char *)pBuffer, 6);			
			uiSrcAddr =	uiSrcAddr + 6 ;

		}

	return 1; 
}
//擦除整个芯片		  
//等待时间超长...
void W25Q32_Erase_Chip(void)   
{                                   
    W25Q32_Write_Enable();                  //SET WEL 
    W25Q32_Wait_Busy();   
	  SPI1_CS_LOW();                            //使能器件   
    SPI1_MasterSendByte(W25X_ChipErase);        //发送片擦除命令  
	  SPI1_CS_HIGH();                            //取消片选     	      
	  W25Q32_Wait_Busy();   				   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个山区的最少时间:150ms
void W25Q32_Erase_Sector(uint32_t Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
  W25Q32_Write_Enable();                  //SET WEL 	 
  W25Q32_Wait_Busy();   
	SPI1_CS_LOW();                            //使能器件   
  SPI1_MasterSendByte(W25X_SectorErase);      //发送扇区擦除指令 
  SPI1_MasterSendByte((uint8_t)((Dst_Addr)>>16));  //发送24bit地址    
  SPI1_MasterSendByte((uint8_t)((Dst_Addr)>>8));   
  SPI1_MasterSendByte((uint8_t)Dst_Addr);  
	SPI1_CS_HIGH();                            //取消片选     	      
  W25Q32_Wait_Busy();   				   //等待擦除完成
}  

//等待空闲
void W25Q32_Wait_Busy(void)   
{   
	while((W25Q32_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  

//进入掉电模式
void W25Q32_PowerDown(void)   
{ 
	  SPI1_CS_LOW();                            //使能器件   
    SPI1_MasterSendByte(W25X_PowerDown);        //发送掉电命令  
	  SPI1_CS_HIGH();                             //取消片选     	      
    DelayMs(3);                               //等待TPD  
}   

//唤醒
void W25Q32_WAKEUP(void)   
{  
	  SPI1_CS_LOW();                             //使能器件   
    SPI1_MasterSendByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	  SPI1_CS_HIGH();                            //取消片选     	      
    DelayMs(3);                               //等待TRES1
}  


/*

uint8_t	Buffer[60] = {
	0X30,0X1B,0X97,0X82,0X00,0X00,
	0X30,0X1B,0X97,0X82,0X00,0X01,
	0X30,0X1B,0X97,0X82,0X00,0X02,
	0X30,0X1B,0X97,0X82,0X00,0X03,	
	0X30,0X1B,0X97,0X82,0X00,0X04,
	0X30,0X1B,0X97,0X82,0X00,0X05,
	0X30,0X1B,0X97,0X82,0X00,0X06,
	0X30,0X1B,0X97,0X82,0X00,0X07,
	0X30,0X1B,0X97,0X82,0X00,0X08,	
	0X30,0X1B,0X97,0X82,0X00,0X09	
};



uint8_t	Buff[60] = {0};

uint8_t	Buffer2[6] = {0X30,0X1B,0X97,0X82,0X00,0X08};



//W25Q32_Write(Buffer,0,60);
//DelayMs(500);
//W25Q32_Read(Buff,0,60)	;
//debug_printf("[<--tcp_beat<--hub] len=%02d: ", 60);
//log_hex((char *)Buff, 60);
// debug_printf("W25Q32_CmpData len=%02d: ", W25Q32_CmpMacData(0, Buffer2,60));
 */
 
 




