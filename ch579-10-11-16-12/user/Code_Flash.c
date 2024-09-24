#define CODE_FLASH_C_

#include "Code_Flash.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

/*
 * CH579 CodeFlash ���50KB �䵱U�̵Ĵ洢��
 * 0x32000 ~ 0x3E7FF 
 * 
 */


/*******************************************************************************
* Function Name  : CodeFlash_WriteBuf
* Description    : CodeFlash �������˫��д��
* Input          : addr: 32λ��ַ����Ҫ4����  
				   pdat: ��д�����ݻ������׵�ַ
				    len: ��д�������ֽڳ���
* Return         : FAILED  - ����
				   SUCCESS - �ɹ�
*******************************************************************************/
UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat)
{
  UINT32  add = addr + START_ADDR;
  UINT8   status = 0;	
	UINT8  i;
	UINT32 dat;
	
	//len = len + 4- len%4;
	
	add = add + 4 - add%4;  //4�ֽڶ���
	
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
* Description    : CodeFlash �������˫�ֶ�
* Input          : addr: 32λ��ַ����Ҫ4����  
				   len: ��д�������ֽڳ���
				   Buf: д���buf
*******************************************************************************/
UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf)
{
	UINT32  add = addr + START_ADDR; 
	UINT8  i;
		
	//len = len + 4- len%4;
	
	add = add + 4- add%4;  //4�ֽڶ���
	
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
* Description    : CodeFlash �������һ�β���512B
* Input          : addr: 32λ��ַ����Ҫ512����		   				
* Return         : FAILED  - ����
				   SUCCESS - �ɹ�
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

//˫��д��
void Code_Flash_Node_Write(uint32_t addr ,uint32_t data)  
{
	uint8_t Node_Number[4] ;
	
	
  Node_Number[0] = (data >> 24);
  Node_Number[1] = (data >> 16);
  Node_Number[2] = (data >> 8);
  Node_Number[3] = data; // ����id	

  CodeFlash_WriteBuf(addr, 4, Node_Number);
}
//˫�ֶ�ȡ
void Code_Flash_Node_Read(uint32_t addr,uint32_t *data)
{
	uint8_t Node_Number[4] ;
	
  CodeFlash_ReadBuf( addr, 4,  Node_Number) ;
	
  *data |= Node_Number[0]<<24 ;
  *data |= Node_Number[1]<<16 ;	
  *data |= Node_Number[2]<<8 ;	
  *data |= Node_Number[3] ;		
}

/* Ƭ��flash��дʵ��
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

 
