#ifndef  _CODE_FLASH_H
#define  _CODE_FLASH_H

#ifndef _CODE_FLASH_C_//如果没有定义
#define _CODE_FLASH_Cx_ extern
#else
#define _CODE_FLASH_Cx_
#endif

#include "CH57x_common.h"

#define START_ADDR  0x3E800
#define NODE_DATAFLASHADDR   0x0400 // 缓存节点总数的首地址

UINT8 CodeFlash_WriteBuf(UINT32 addr, UINT16 len,PUINT8 pdat);
UINT8 CodeFlash_ReadBuf(UINT32 addr, UINT16 len, PUINT8 Buf);
UINT8 CodeFlash_BlockErase(UINT32 addr);

void Code_Flash_Node_Write(uint32_t addr ,uint32_t data) ;
void Code_Flash_Node_Read(uint32_t addr,uint32_t *data);
void Flash_Node_Read_Init(void) ;

#endif
