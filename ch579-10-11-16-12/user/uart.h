#ifndef __UART_H
#define __UART_H

#ifndef UART_C_//���û�ж���
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
/****************************����1****************************/
//���ջ��ζ���
UART_Ex_ rb_t rb_t_usart1_read;
#define rb_t_usart1_read_buff_len 64
UART_Ex_ unsigned char rb_t_usart1_read_buff[rb_t_usart1_read_buff_len];
//�ӻ��濽������ʹ��
UART_Ex_ unsigned char usart1_read_buff_copy[rb_t_usart1_read_buff_len];
//���ռ���
UART_Ex_ int usart1_read_count_copy;
//���յ�һ֡���ݱ�־
UART_Ex_ unsigned char usart1_idle_flag;//��־

//���ͻ��ζ���
UART_Ex_ rb_t rb_t_usart1_send;
#define rb_t_usart1_send_buff_len 64
UART_Ex_ unsigned char rb_t_usart1_send_buff[rb_t_usart1_send_buff_len];
UART_Ex_ unsigned char rb_t_usart1_send_byte;//������ȡ���ζ���1���ֽ�
#endif


#if UART0_ENABLE
/****************************����0****************************/
//���ջ��ζ���
UART_Ex_ rb_t rb_t_usart0_read;
#define rb_t_usart0_read_buff_len 256
UART_Ex_ unsigned char rb_t_usart0_read_buff[rb_t_usart0_read_buff_len];
//�ӻ��濽������ʹ��
UART_Ex_ unsigned char usart0_read_buff_copy[rb_t_usart0_read_buff_len];
//���ռ���
UART_Ex_ int usart0_read_count_copy;
//���յ�һ֡���ݱ�־
UART_Ex_ unsigned char usart0_idle_flag;//��־

//���ͻ��ζ���
UART_Ex_ rb_t rb_t_usart0_send;
#define rb_t_usart0_send_buff_len 256
UART_Ex_ unsigned char rb_t_usart0_send_buff[rb_t_usart0_send_buff_len];
UART_Ex_ unsigned char rb_t_usart0_send_byte;//������ȡ���ζ���1���ֽ�
#endif


#if UART3_ENABLE
/****************************����3****************************/
//���ջ��ζ���
UART_Ex_ rb_t rb_t_usart3_read;
#define rb_t_usart3_read_buff_len 256
UART_Ex_ unsigned char rb_t_usart3_read_buff[rb_t_usart3_read_buff_len];
//�ӻ��濽������ʹ��
UART_Ex_ unsigned char usart3_read_buff_copy[rb_t_usart3_read_buff_len];
//���ռ���
UART_Ex_ int usart3_read_count_copy;
//���յ�һ֡���ݱ�־
UART_Ex_ unsigned char usart3_idle_flag;//��־

//���ͻ��ζ���
UART_Ex_ rb_t rb_t_usart3_send;
#define rb_t_usart3_send_buff_len 256
UART_Ex_ unsigned char rb_t_usart3_send_buff[rb_t_usart3_send_buff_len];
UART_Ex_ unsigned char rb_t_usart3_send_byte;//������ȡ���ζ���1���ֽ�
#endif

void uart_init(void);

void LORA_Init(void) ;
void LoRa_SendData(volatile unsigned char *pdate,volatile unsigned char *pmac,unsigned char lv8u_tx_length);
/**
* @brief  ���ڷ����ַ�������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_bytes(char USARTx, unsigned char *c,uint32_t cnt);
/**
* @brief  ���ڷ���һ���ֽ�
* @param  *c:���͵�����ָ��  cnt:���ݸ���
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_byte(char USARTx, unsigned char c);

/**
* @brief  �����жϷ�ʽ��������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
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

