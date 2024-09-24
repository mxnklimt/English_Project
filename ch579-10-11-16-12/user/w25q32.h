#ifndef W25Q32_H_
#define W25Q32_H_

#ifndef W25Q32_C_//如果没有定义
#define W25Q32_Cx_ extern
#else
#define W25Q32_Cx_
#endif

#include "CH57x_common.h"

#define  SPI1_CS_LOW()        GPIOA_ResetBits( GPIO_Pin_6 )
#define  SPI1_CS_HIGH()       GPIOA_SetBits( GPIO_Pin_6 )


//指令表
#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable		  0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		  0x0B 
#define W25X_FastReadDual		  0x3B 
#define W25X_PageProgram		  0x02 
#define W25X_BlockErase			  0xD8 
#define W25X_SectorErase		  0x20 
#define W25X_ChipErase			  0xC7 
#define W25X_PowerDown			  0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 


/*config pin*/
uint16_t W25Q32_ReadID(void);
void SPI_W25Q32_Init(void);
uint8_t W25Q32_ReadSR(void)  ;
void W25Q32_Write_SR(uint8_t sr) ;
void W25Q32_Write_Disable(void)  ;
void W25Q32_Wait_Busy(void) ;
void W25Q32_Read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead) ;
void W25Q32_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q32_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q32_Write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void W25Q32_Erase_Chip(void);    	  	//整片擦除
void W25Q32_Erase_Sector(uint32_t Dst_Addr);	//扇区擦除
void W25Q32_Wait_Busy(void);           	//等待空闲
void W25Q32_PowerDown(void);        	//进入掉电模式
void W25Q32_WAKEUP(void);				//唤醒
uint8_t W25Q32_CmpData(uint8_t* pBuffer,uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize);
uint8_t W25Q32_CmpMacData(uint32_t _uiSrcAddr, uint8_t *_ucpTar,uint32_t _uiSize);

#endif
