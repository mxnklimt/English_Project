#define W25Q32_C_

#include "w25q32.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

//��ʼ��SPI1 FLASH��IO��
void SPI_W25Q32_Init(void)
{ 
	GPIOA_SetBits( GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6 );
	GPIOA_ModeCfg( GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_6, GPIO_ModeOut_PP_5mA );	// MOSI/SCK/CS
	GPIOA_SetBits( GPIO_Pin_2 );
	GPIOA_ModeCfg( GPIO_Pin_2, GPIO_ModeIN_PU );	// MISO	
	
  SPI1_MasterDefInit();
  SPI1_DataMode( Mode3_HighBitINFront );
  SPI1_CLKCfg( 4 );
  debug_printf("id:0x%04x\r\n", W25Q32_ReadID() );//��ȡFLASH ID.
	
		
}  

//��ȡоƬID		     
uint16_t W25Q32_ReadID(void)
{
	uint16_t Temp = 0;	  
	SPI1_CS_LOW();				    
  SPI1_MasterSendByte(CMD_DEVICE_ID);                    //��ȡID����
  SPI1_MasterSendByte(0x00);
  SPI1_MasterSendByte(0x00);
  SPI1_MasterSendByte(0x00);	
	
	Temp|=SPI1_MasterRecvByte()<<8;  
	Temp|=SPI1_MasterRecvByte();	 
  SPI1_CS_HIGH();				    
	return Temp;
}  

//��ȡW25QXX��״̬�Ĵ���
//Ĭ��:0x00
uint8_t W25Q32_ReadSR(void)   
{  
	uint8_t byte=0;  
	
	SPI1_CS_LOW();                             //ʹ������   
	SPI1_MasterSendByte(CMD_STATUS1);    //���Ͷ�ȡ״̬�Ĵ�������    
	byte=SPI1_MasterRecvByte();             //��ȡһ���ֽ�  
  SPI1_CS_HIGH();                           //ȡ��Ƭѡ     
	return byte;   
} 


//дW25QXX״̬�Ĵ���
//ֻ��SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)����д!!!
void W25Q32_Write_SR(uint8_t sr)   
{   
	SPI1_CS_LOW();                            //ʹ������   
	SPI1_MasterSendByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ�������    
	SPI1_MasterSendByte(sr);               //д��һ���ֽ�  
  SPI1_CS_HIGH();                             //ȡ��Ƭѡ     	      
}  

//W25QXXдʹ��	
//��WEL��λ   
void W25Q32_Write_Enable(void)   
{
	SPI1_CS_LOW();                            //ʹ������   
  SPI1_MasterSendByte(W25X_WriteEnable);      //����дʹ��  
  SPI1_CS_HIGH();                             //ȡ��Ƭѡ     	      
} 
//W25QXXд��ֹ	
//��WEL����  
void W25Q32_Write_Disable(void)   
{  
	SPI1_CS_LOW();                           //ʹ������   
  SPI1_MasterSendByte(W25X_WriteDisable);     //����д��ָֹ��    
  SPI1_CS_HIGH();                            //ȡ��Ƭѡ     	      
} 

//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ�����ȵ�����
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ�ĵ�ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
void W25Q32_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead)   
{ 
 	  uint16_t i;   										    
	  SPI1_CS_LOW();                              //ʹ������   
    SPI1_MasterSendByte(W25X_ReadData);         //���Ͷ�ȡ����   
    SPI1_MasterSendByte((uint8_t)((ReadAddr)>>16));  //����24bit��ַ    
    SPI1_MasterSendByte((uint8_t)((ReadAddr)>>8));   
    SPI1_MasterSendByte((uint8_t)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	   { 
        pBuffer[i]=SPI1_MasterRecvByte();   //ѭ������  
     }
	  SPI1_CS_HIGH();  				    	      
} 

//SPI��һҳ(0~65535)��д������256���ֽڵ�����
//��ָ����ַ��ʼд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
void W25Q32_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
 	uint16_t i;  
  W25Q32_Write_Enable();                  //SET WEL 
	SPI1_CS_LOW();                            //ʹ������   
  SPI1_MasterSendByte(W25X_PageProgram);      //����дҳ����   
  SPI1_MasterSendByte((uint8_t)((WriteAddr)>>16)); //����24bit��ַ    
  SPI1_MasterSendByte((uint8_t)((WriteAddr)>>8));   
  SPI1_MasterSendByte((uint8_t)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)SPI1_MasterSendByte(pBuffer[i]);//ѭ��д��  
	SPI1_CS_HIGH();                            //ȡ��Ƭѡ 
	W25Q32_Wait_Busy();					   //�ȴ�д�����
} 

//�޼���дSPI FLASH 
//����ȷ����д�ĵ�ַ��Χ�ڵ�����ȫ��Ϊ0XFF,�����ڷ�0XFF��д������ݽ�ʧ��!
//�����Զ���ҳ���� 
//��ָ����ַ��ʼд��ָ�����ȵ�����,����Ҫȷ����ַ��Խ��!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���65535)
//CHECK OK
void W25Q32_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 			 		 
	uint16_t pageremain;	   
	pageremain=256-WriteAddr%256; //��ҳʣ����ֽ���		 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//������256���ֽ�
	while(1)
	{	   
		W25Q32_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//д�������
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite>256)pageremain=256; //һ�ο���д��256���ֽ�
			else pageremain=NumByteToWrite; 	  //����256���ֽ���
		}
	};	    
} 
//дSPI FLASH  
//��ָ����ַ��ʼд��ָ�����ȵ�����
//�ú�������������!
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд��ĵ�ַ(24bit)						
//NumByteToWrite:Ҫд����ֽ���(���65535)   
uint8_t W25Q32_BUFFER[4096];		 
void W25Q32_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{ 
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;	   
 	uint16_t i;    
	uint8_t * W25Q32_BUF;	  
   	W25Q32_BUF=W25Q32_BUFFER;	     
 	secpos=WriteAddr/4096;//������ַ  
	secoff=WriteAddr%4096;//�������ڵ�ƫ��
	secremain=4096-secoff;//����ʣ��ռ��С   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//������
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//������4096���ֽ�
	while(1) 
	{	
		W25Q32_Read(W25Q32_BUF,secpos*4096,4096);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(W25Q32_BUF[secoff+i]!=0XFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			W25Q32_Erase_Sector(secpos);//�����������
			for(i=0;i<secremain;i++)	   //����
			{
				W25Q32_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q32_Write_NoCheck(W25Q32_BUF,secpos*4096,4096);//д����������  

		}else W25Q32_Write_NoCheck(pBuffer,WriteAddr,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumByteToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;//������ַ��1
			secoff=0;//ƫ��λ��Ϊ0 	 

		   	pBuffer+=secremain;  //ָ��ƫ��
			WriteAddr+=secremain;//д��ַƫ��	   
		   	NumByteToWrite-=secremain;				//�ֽ����ݼ�
			if(NumByteToWrite>4096)secremain=4096;	//��һ����������д����
			else secremain=NumByteToWrite;			//��һ����������д����
		}	 
	};	 
}
/*
*********************************************************************************************************
*	�� �� ��: sf_CmpData
*	����˵��: �Ƚ�Flash�����ݡ�
*	��    ��:  	_ucpTar : ���ݻ�������
*				_uiSrcAddr ��Flash��ַ��
*				_uiSize �����ݸ���, ���ܳ���оƬ��������
*	�� �� ֵ: 0 = ���, 1 = ����
*********************************************************************************************************
*/
uint8_t W25Q32_CmpData(uint8_t* pBuffer,uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
	uint16_t i;


	SPI1_CS_LOW(); 									/* ʹ��Ƭѡ */
  SPI1_MasterSendByte(W25X_ReadData);         //���Ͷ�ȡ����   
  SPI1_MasterSendByte((uint8_t)((_uiSrcAddr)>>16));  //����24bit��ַ    
  SPI1_MasterSendByte((uint8_t)((_uiSrcAddr)>>8));   
  SPI1_MasterSendByte((uint8_t)_uiSrcAddr);   
  for(i=0;i<_uiSize;i++)
	 { 
     pBuffer[i]=SPI1_MasterRecvByte();   //ѭ������  
		 if(pBuffer[i] != *_ucpTar++)
			{
			  debug_printf("W25Q32_CmpData i=%02d: len=%02d: ",i ,*_ucpTar);
				goto NOTEQ;		       /* ����� */
			}
   }

  SPI1_CS_HIGH();
	return 0;		/* ��� */
	
NOTEQ:	
	SPI1_CS_HIGH();	/* ����� */
	return 1;
}

/*
*********************************************************************************************************
*	�� �� ��: sf_CmpData
*	����˵��: �Ƚ�Flash�����ݡ�
*	��    ��:  	_ucpTar : ���ݻ�������
*				_uiSrcAddr ��Flash��ַ��
*				_uiSize �����ݸ���, ���ܳ���оƬ��������
*	�� �� ֵ: 0 = ���, 1 = ����
*********************************************************************************************************
*/
uint8_t W25Q32_CmpMacData(uint32_t _uiSrcAddr, uint8_t *_ucpTar,uint32_t _uiSize)
{
	uint16_t i , j ;
  uint8_t pBuffer[6] = {0};
  uint32_t uiSrcAddr = _uiSrcAddr ;
 
  for(i=0;i<_uiSize;i=i+6)
	 { 
		 SPI1_CS_LOW();						                  /* ʹ��Ƭѡ */
     SPI1_MasterSendByte(W25X_ReadData);                //���Ͷ�ȡ����   
     SPI1_MasterSendByte((uint8_t)((uiSrcAddr)>>16));  //����24bit��ַ    
     SPI1_MasterSendByte((uint8_t)((uiSrcAddr)>>8));   
     SPI1_MasterSendByte((uint8_t)uiSrcAddr);		 
     debug_printf("W25Q32_CmpData55555 len=%02d: \r\n", uiSrcAddr); 		 
	   for(j=0;j<6;j++)	 
	    {
				pBuffer[j]=SPI1_MasterRecvByte();   //ѭ������  	
        if(pBuffer[j] != _ucpTar[j])
			   {	  
				    goto NOTEQ;		       /* ����� */
			   }				
		  }
			SPI1_CS_HIGH();
			return i;                   /* ��� */
			NOTEQ:
			SPI1_CS_HIGH();	/* ����� */
      debug_printf("W25Q32_CmpData55555 len=%02d: \r\n", 6);
      log_hex((char *)pBuffer, 6);			
			uiSrcAddr =	uiSrcAddr + 6 ;

		}

	return 1; 
}
//��������оƬ		  
//�ȴ�ʱ�䳬��...
void W25Q32_Erase_Chip(void)   
{                                   
    W25Q32_Write_Enable();                  //SET WEL 
    W25Q32_Wait_Busy();   
	  SPI1_CS_LOW();                            //ʹ������   
    SPI1_MasterSendByte(W25X_ChipErase);        //����Ƭ��������  
	  SPI1_CS_HIGH();                            //ȡ��Ƭѡ     	      
	  W25Q32_Wait_Busy();   				   //�ȴ�оƬ��������
}   
//����һ������
//Dst_Addr:������ַ ����ʵ����������
//����һ��ɽ��������ʱ��:150ms
void W25Q32_Erase_Sector(uint32_t Dst_Addr)   
{  
	//����falsh�������,������   
 	printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096;
  W25Q32_Write_Enable();                  //SET WEL 	 
  W25Q32_Wait_Busy();   
	SPI1_CS_LOW();                            //ʹ������   
  SPI1_MasterSendByte(W25X_SectorErase);      //������������ָ�� 
  SPI1_MasterSendByte((uint8_t)((Dst_Addr)>>16));  //����24bit��ַ    
  SPI1_MasterSendByte((uint8_t)((Dst_Addr)>>8));   
  SPI1_MasterSendByte((uint8_t)Dst_Addr);  
	SPI1_CS_HIGH();                            //ȡ��Ƭѡ     	      
  W25Q32_Wait_Busy();   				   //�ȴ��������
}  

//�ȴ�����
void W25Q32_Wait_Busy(void)   
{   
	while((W25Q32_ReadSR()&0x01)==0x01);   // �ȴ�BUSYλ���
}  

//�������ģʽ
void W25Q32_PowerDown(void)   
{ 
	  SPI1_CS_LOW();                            //ʹ������   
    SPI1_MasterSendByte(W25X_PowerDown);        //���͵�������  
	  SPI1_CS_HIGH();                             //ȡ��Ƭѡ     	      
    DelayMs(3);                               //�ȴ�TPD  
}   

//����
void W25Q32_WAKEUP(void)   
{  
	  SPI1_CS_LOW();                             //ʹ������   
    SPI1_MasterSendByte(W25X_ReleasePowerDown);   //  send W25X_PowerDown command 0xAB    
	  SPI1_CS_HIGH();                            //ȡ��Ƭѡ     	      
    DelayMs(3);                               //�ȴ�TRES1
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
 
 




