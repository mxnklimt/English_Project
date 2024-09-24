#define CODE_FLASH_C_

#include "Code_Flash.h"

#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

/*
 * CH579 CodeFlash 最后50KB 充当U盘的存储区
 * 0x32000 ~ 0x3E7FF 
 * 
 */


/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash 连续多个双字写入
* Input          : addr: 32位地址，需要4对齐  
				   pdat: 待写入数据缓存区首地址
				    len: 待写入数据字节长度
* Return         : FAILED  - 错误
				   SUCCESS - 成功
*******************************************************************************/
UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat)
{
  UINT32  add = addr + START_ADDR;
  UINT8   status = 0;	
	UINT8  i;
	UINT32 dat;
	
	//len = len + 4- len%4;
	
	add = add + 4 - add%4;  //4字节对齐
	
  EnableCodeFlash();
  for(i=0; i<len; i+=4)
   {
		dat = *(pdat+3+i);
		dat = dat<<8 | *(pdat+2+i);
		dat = dat<<8 | *(pdat+1+i);
		dat = dat<<8 | *(pdat+i);		
		//printf("dat = %08lx \n" ,dat);
		status = FlashWriteDW( add, dat );        	
        add += 4;
   }
  LockFlashALL();
  return ( status );
}

/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash 连续多个双字读
* Input          : addr: 32位地址，需要4对齐  
				   len: 待写入数据字节长度
				   Buf: 写入的buf
*******************************************************************************/
UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf)
{
	UINT32  add = addr + START_ADDR; 
	UINT8  i;
		
	//len = len + 4- len%4;
	
	add = add + 4- add%4;  //4字节对齐
	
    for(i=0; i<len; i+=4)
     {
       *(Buf+i) = *(PUINT32)add & 0xff;
		   *(Buf+i+1) = *(PUINT32)add >> 8 & 0xff;
		   *(Buf+i+2) = *(PUINT32)add >> 16 & 0xff;
		   *(Buf+i+3) = *(PUINT32)add >> 24 & 0xff;		
       add += 4;
    }
	return SUCCESS;
}

/*******************************************************************************
* Function Name  : CodeFlash_BlockErase
* Description    : CodeFlash 块擦除，一次擦除512B
* Input          : addr: 32位地址，需要512对齐		   				
* Return         : FAILED  - 错误
				   SUCCESS - 成功
*******************************************************************************/
UINT8 CodeFlash_BlockErase(UINT32 addr)
{
  UINT8  status = 0;
	UINT32 add = addr + START_ADDR;

    EnableCodeFlash();
    status = FlashBlockErase( add );
    LockFlashALL();
    return ( status );
}

//双字写入
void Code_Flash_Node_Write(uint32_t addr ,uint32_t data)  
{
	uint8_t Node_Number[4] ;
	
	
  Node_Number[0] = (data >> 24);
  Node_Number[1] = (data >> 16);
  Node_Number[2] = (data >> 8);
  Node_Number[3] = data; // 数据id	

  CodeFlash_WriteBuf(addr, 4, Node_Number);
}
//双字读取
void Code_Flash_Node_Read(uint32_t addr,uint32_t *data)
{
	uint8_t Node_Number[4] ;
	
  CodeFlash_ReadBuf( addr, 4,  Node_Number) ;
	
  *data |= Node_Number[0]<<24 ;
  *data |= Node_Number[1]<<16 ;	
  *data |= Node_Number[2]<<8 ;	
  *data |= Node_Number[3] ;		
}

/* 片上flash读写实例
  uint32_t num=0;
	Code_Flash_Node_Write(0x00000400 , 0x12345678);  
	DelayMs(500);
  Code_Flash_Node_Read(0x00000400,&num);
  debug_printf("num=%02x: ", num);
*/

void Flash_Node_Read_Init(void)
{	
   Code_Flash_Node_Read(NODE_DATAFLASHADDR ,&Node_counter) ;
   debug_printf("Current_Node=%x\r\n", Node_counter);
}

 
