#ifndef __UART_H
#define __UART_H

#ifndef UART_C_//如果没有定义
#define UART_Ex_ extern
#else
#define UART_Ex_
#endif


#include "CH57x_common.h"
#include "LoopList.h"

#define UART0_ENABLE    1
#define UART1_ENABLE    0
#define UART2_ENABLE    0
#define UART3_ENABLE    1

#if UART1_ENABLE
/****************************串口1****************************/
//接收环形队列
UART_Ex_ rb_t rb_t_usart1_read;
#define rb_t_usart1_read_buff_len 64
UART_Ex_ unsigned char rb_t_usart1_read_buff[rb_t_usart1_read_buff_len];
//从缓存拷贝数据使用
UART_Ex_ unsigned char usart1_read_buff_copy[rb_t_usart1_read_buff_len];
//接收计数
UART_Ex_ int usart1_read_count_copy;
//接收到一帧数据标志
UART_Ex_ unsigned char usart1_idle_flag;//标志

//发送环形队列
UART_Ex_ rb_t rb_t_usart1_send;
#define rb_t_usart1_send_buff_len 64
UART_Ex_ unsigned char rb_t_usart1_send_buff[rb_t_usart1_send_buff_len];
UART_Ex_ unsigned char rb_t_usart1_send_byte;//串口提取环形队列1个字节
#endif


#if UART0_ENABLE
/****************************串口0****************************/
//接收环形队列
UART_Ex_ rb_t rb_t_usart0_read;
#define rb_t_usart0_read_buff_len 256
UART_Ex_ unsigned char rb_t_usart0_read_buff[rb_t_usart0_read_buff_len];
//从缓存拷贝数据使用
UART_Ex_ unsigned char usart0_read_buff_copy[rb_t_usart0_read_buff_len];
//接收计数
UART_Ex_ int usart0_read_count_copy;
//接收到一帧数据标志
UART_Ex_ unsigned char usart0_idle_flag;//标志

//发送环形队列
UART_Ex_ rb_t rb_t_usart0_send;
#define rb_t_usart0_send_buff_len 256
UART_Ex_ unsigned char rb_t_usart0_send_buff[rb_t_usart0_send_buff_len];
UART_Ex_ unsigned char rb_t_usart0_send_byte;//串口提取环形队列1个字节
#endif


#if UART3_ENABLE
/****************************串口3****************************/
//接收环形队列
UART_Ex_ rb_t rb_t_usart3_read;
#define rb_t_usart3_read_buff_len 256
UART_Ex_ unsigned char rb_t_usart3_read_buff[rb_t_usart3_read_buff_len];
//从缓存拷贝数据使用
UART_Ex_ unsigned char usart3_read_buff_copy[rb_t_usart3_read_buff_len];
//接收计数
UART_Ex_ int usart3_read_count_copy;
//接收到一帧数据标志
UART_Ex_ unsigned char usart3_idle_flag;//标志

//发送环形队列
UART_Ex_ rb_t rb_t_usart3_send;
#define rb_t_usart3_send_buff_len 256
UART_Ex_ unsigned char rb_t_usart3_send_buff[rb_t_usart3_send_buff_len];
UART_Ex_ unsigned char rb_t_usart3_send_byte;//串口提取环形队列1个字节
#endif

void uart_init(void);

void LORA_Init(void) ;
void LoRa_SendData(volatile unsigned char *pdate,volatile unsigned char *pmac,unsigned char lv8u_tx_length);
/**
* @brief  串口发送字符串数据
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_bytes(char USARTx, unsigned char *c,uint32_t cnt);
/**
* @brief  串口发送一个字节
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_byte(char USARTx, unsigned char c);

/**
* @brief  串口中断方式发送数据
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_byte_it(char USARTx, unsigned char *c,uint32_t cnt);

void log_hex(char *buf,int len);



#define _DEBUG

#ifdef _DEBUG
#define log(...) do {\
 						   	printf(__VA_ARGS__);\
                           }while(0==1)
#else
	#define log(...) 	
#endif	
						   
#endif

