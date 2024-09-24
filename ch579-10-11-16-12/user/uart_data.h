#ifndef UART_DATA_H_
#define UART_DATA_H_

#ifndef UART_DATA_C_//如果没有定义
#define UART_DATA_Cx_ extern
#else
#define UART_DATA_Cx_
#endif

#include "CH57x_common.h"

UART_DATA_Cx_ unsigned char MACnet[6];                 // 缓存数据,全局通用  
UART_DATA_Cx_ uint32_t  main_len;      //全局通用变量
UART_DATA_Cx_ uint32_t Node_counter;                     // 全局通用变量
/*config pin*/

uint8_t sum_make_verify(uint8_t* pBuf, uint16_t len) ;
void bind_set(volatile unsigned char *mac_address)  ;
void unbind_set(volatile unsigned char *mac_address)  ;
void uart_data(void) ;
#endif
