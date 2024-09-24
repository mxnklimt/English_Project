#define UART_C_

#include "uart.h"
#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

static unsigned char LORA_INIT[12]  = {0x68, 0xAA, 0x0C, 0x14, 0x13, 0x24,0x35, 0x46, 0x57, 0x68, 0x91, 0x16};
static unsigned char LORA_DOUBLE[7] = {0x68, 0xAA, 0X07, 0X0F, 0X01, 0X17, 0X16};
static unsigned char LORA_SAVE[7]   = {0x68, 0xAA, 0X07, 0X13, 0X01, 0X1B, 0X16};

#if UART1_ENABLE
/****************************串口1****************************/
//接收环形队列
rb_t rb_t_usart1_read;
//环形队列缓存数组
unsigned char rb_t_usart1_read_buff[rb_t_usart1_read_buff_len];
//从缓存拷贝数据使用
unsigned char usart1_read_buff_copy[rb_t_usart1_read_buff_len];
//接收计数
int usart1_read_count=0;
int usart1_read_count_copy=0;
//接收到一帧数据标志
unsigned char usart1_idle_flag=0;


//发送环形队列
rb_t rb_t_usart1_send;
//环形队列缓存数组
unsigned char rb_t_usart1_send_buff[rb_t_usart1_send_buff_len];
//串口提取环形队列1个字节
unsigned char rb_t_usart1_send_byte;
#endif



#if UART0_ENABLE
/****************************串口0****************************/
//接收环形队列
rb_t rb_t_usart0_read;
//环形队列缓存数组
unsigned char rb_t_usart0_read_buff[rb_t_usart0_read_buff_len];
//从缓存拷贝数据使用
unsigned char usart0_read_buff_copy[rb_t_usart0_read_buff_len];
//接收计数
int usart0_read_count=0;
int usart0_read_count_copy=0;
//接收到一帧数据标志
unsigned char usart0_idle_flag=0;

//发送环形队列
rb_t rb_t_usart0_send;
//环形队列缓存数组
unsigned char rb_t_usart0_send_buff[rb_t_usart0_send_buff_len];
//串口提取环形队列1个字节
unsigned char rb_t_usart0_send_byte;
#endif

#if UART3_ENABLE
/****************************串口3****************************/
//接收环形队列
rb_t rb_t_usart3_read;
//环形队列缓存数组
unsigned char rb_t_usart3_read_buff[rb_t_usart3_read_buff_len];
//从缓存拷贝数据使用
unsigned char usart3_read_buff_copy[rb_t_usart3_read_buff_len];
//接收计数
int usart3_read_count=0;
int usart3_read_count_copy=0;
//接收到一帧数据标志
unsigned char usart3_idle_flag=0;

//发送环形队列
rb_t rb_t_usart3_send;
//环形队列缓存数组
unsigned char rb_t_usart3_send_buff[rb_t_usart3_send_buff_len];
//串口提取环形队列1个字节
unsigned char rb_t_usart3_send_byte;
#endif 

void uart_init(void)
{
#if UART1_ENABLE
	/*配置串口1*/
	//创建环形队列(用于串口1接收数据)
	rbCreate(&rb_t_usart1_read,rb_t_usart1_read_buff,rb_t_usart1_read_buff_len);
	//创建环形队列(用于串口1发送数据)
	rbCreate(&rb_t_usart1_send,rb_t_usart1_send_buff,rb_t_usart1_send_buff_len);
	
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);     // RXD-配置上拉输入
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);// TXD-配置推挽输出，注意先让IO口输出高电平
	UART1_DefInit();
	/*使能接收中断,接收错误中断*/
	UART1_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	//	NVIC_SetPriority(TMR0_IRQn,18);//设置中断优先级(也可以不设置,默认14)
	NVIC_EnableIRQ( UART1_IRQn );
#endif 
	
#if UART0_ENABLE	
	/*配置串口0*/
	//创建环形队列(用于串口接收数据)
	rbCreate(&rb_t_usart0_read, rb_t_usart0_read_buff, rb_t_usart0_read_buff_len);
	//创建环形队列(用于串口发送数据)
	rbCreate(&rb_t_usart0_send, rb_t_usart0_send_buff, rb_t_usart0_send_buff_len);
	
	GPIOPinRemap(ENABLE,RB_PIN_UART0); //重定向引脚
	GPIOA_SetBits(GPIO_Pin_14);//TX输出高电平
	GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);     // RXD-配置上拉输入
	GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA);// TXD-配置推挽输出，注意先让IO口输出高电平
	UART0_DefInit();
	/*使能接收中断,接收错误中断*/
	UART0_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	//	NVIC_SetPriority(TMR0_IRQn,18);//设置中断优先级(也可以不设置,默认14)
	NVIC_EnableIRQ( UART0_IRQn );
#endif

#if UART3_ENABLE
	/*配置串口3*/
	//创建环形队列(用于串口接收数据)
	rbCreate(&rb_t_usart3_read, rb_t_usart3_read_buff, rb_t_usart3_read_buff_len);
	//创建环形队列(用于串口发送数据)
	rbCreate(&rb_t_usart3_send, rb_t_usart3_send_buff, rb_t_usart3_send_buff_len);
	
	GPIOPinRemap(ENABLE,RB_PIN_UART3); //重定向引脚
	GPIOB_SetBits(GPIO_Pin_21);
	GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU);     // RXD-配置上拉输入
	GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA);// TXD-配置推挽输出，注意先让IO口输出高电平
	UART3_DefInit();
	/*使能接收中断,接收错误中断*/
	UART3_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	NVIC_EnableIRQ( UART3_IRQn );
#endif
}



/**
* @brief  串口发送字符串数据
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_bytes(char USARTx, unsigned char *c,uint32_t cnt)
{
	switch(USARTx)
	{
		case 0: UART0_SendString(c,cnt); break;
		case 1: UART1_SendString(c,cnt); break;
		case 2: UART2_SendString(c,cnt); break;
		case 3: UART3_SendString(c,cnt); break;
	}
}


/**
* @brief  串口发送一个字节
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_byte(char USARTx, unsigned char c)
{
	switch(USARTx)
	{
		case 0: UART0_SendByte(c); break;
		case 1: UART1_SendByte(c); break;
		case 2: UART2_SendByte(c); break;
		case 3: UART3_SendByte(c); break;
	}
}


/**
* @brief  串口中断方式发送数据
* @param  *c:发送的数据指针  cnt:数据个数
* @param  None
* @param  None
* @retval None
* @example 
**/
void uart_send_byte_it(char USARTx, unsigned char *c,uint32_t cnt)
{
	switch(USARTx)
	{
		case 0: 
			PutData(&rb_t_usart0_send,c,cnt);
			UART0_INTCfg( ENABLE,RB_IER_THR_EMPTY );//使能中断发送
			break;
		case 1: 
			#if UART1_ENABLE
			PutData(&rb_t_usart1_send,c,cnt);
			UART1_INTCfg( ENABLE,RB_IER_THR_EMPTY );//使能中断发送
			#endif
			break;
		case 2: 
			#if UART2_ENABLE
			PutData(&rb_t_usart2_send,c,cnt);
			UART2_INTCfg( ENABLE,RB_IER_THR_EMPTY );//使能中断发送
			#endif			
			break;
		case 3:  
			PutData(&rb_t_usart3_send,c,cnt);
			UART3_INTCfg( ENABLE,RB_IER_THR_EMPTY );//使能中断发送
			break;
	}
}

#if UART0_ENABLE
/*串口中断*/
void UART0_IRQHandler(void)
{
	char data;
	switch( UART0_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // 线路状态错误
				UART0_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // 数据达到设置触发点
				data = UART0_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart0_read,&data,1);
				usart0_read_count++;
				break;
		case UART_II_RECV_TOUT:         // 接收超时，暂时一帧数据接收完成
				data = UART0_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart0_read,&data,1);
				usart0_read_count++;
				usart0_read_count_copy = usart0_read_count;
				usart0_read_count=0;
				usart0_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // 发送缓存区空，可继续发送
				if(rbCanRead(&rb_t_usart0_send)>0)//如果里面的数据个数大于0
				{
					rbRead(&rb_t_usart0_send,&rb_t_usart0_send_byte,1);
					UART0_SendByte(rb_t_usart0_send_byte);
				}
				else
				{
					UART0_INTCfg( DISABLE,RB_IER_THR_EMPTY );//关闭中断发送
				}
				break;
		case UART_II_MODEM_CHG:         // 只支持串口0
				break;
		default:
				break;
	}
}
#endif

#if UART2_ENABLE
/*串口中断*/
void UART2_IRQHandler(void)
{
	char data;
	switch( UART2_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // 线路状态错误
				UART2_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // 数据达到设置触发点
				data = UART2_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart2_read,&data,1);
				usart2_read_count++;
				break;
		case UART_II_RECV_TOUT:         // 接收超时，暂时一帧数据接收完成
				data = UART2_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart2_read,&data,1);
				usart2_read_count++;
				usart2_read_count_copy = usart2_read_count;
				usart2_read_count=0;
				usart2_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // 发送缓存区空，可继续发送
				if(rbCanRead(&rb_t_usart2_send)>0)//如果里面的数据个数大于0
				{
					rbRead(&rb_t_usart2_send,&rb_t_usart2_send_byte,1);
					UART2_SendByte(rb_t_usart2_send_byte);
				}
				else
				{
					UART2_INTCfg( DISABLE,RB_IER_THR_EMPTY );//关闭中断发送
				}
				break;
		case UART_II_MODEM_CHG:         // 只支持串口0
				break;
		default:
				break;
	}
}
#endif

#if UART1_ENABLE
/*串口中断*/
void UART1_IRQHandler(void)
{
	char data;
	switch( UART1_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // 线路状态错误
				UART1_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // 数据达到设置触发点
				data = UART1_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart1_read,&data,1);
				usart1_read_count++;
				break;
		case UART_II_RECV_TOUT:         // 接收超时，暂时一帧数据接收完成
				data = UART1_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart1_read,&data,1);
				usart1_read_count++;
				usart1_read_count_copy = usart1_read_count;
				usart1_read_count=0;
				usart1_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // 发送缓存区空，可继续发送
				if(rbCanRead(&rb_t_usart1_send)>0)//如果里面的数据个数大于0
				{
					rbRead(&rb_t_usart1_send,&rb_t_usart1_send_byte,1);
					UART1_SendByte(rb_t_usart1_send_byte);
				}
				else
				{
					UART1_INTCfg( DISABLE,RB_IER_THR_EMPTY );//关闭中断发送
				}
				break;
		case UART_II_MODEM_CHG:         // 只支持串口0
				break;
		default:
				break;
	}
}
#endif

#if UART3_ENABLE
/*串口中断*/
void UART3_IRQHandler(void)
{
	char data;
	switch( UART3_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // 线路状态错误
				UART3_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // 数据达到设置触发点
				data = UART3_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart3_read,&data,1);
				usart3_read_count++;
				break;
		case UART_II_RECV_TOUT:         // 接收超时，暂时一帧数据接收完成
				data = UART3_RecvByte();//读取串口数据
				/*串口数据存储到缓存*/
				PutData(&rb_t_usart3_read,&data,1);
				usart3_read_count++;
				usart3_read_count_copy = usart3_read_count;
				usart3_read_count=0;
				usart3_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // 发送缓存区空，可继续发送
				if(rbCanRead(&rb_t_usart3_send)>0)//如果里面的数据个数大于0
				{
					rbRead(&rb_t_usart3_send,&rb_t_usart3_send_byte,1);
					UART3_SendByte(rb_t_usart3_send_byte);
				}
				else
				{
					UART3_INTCfg( DISABLE,RB_IER_THR_EMPTY );//关闭中断发送
				}
				break;
		case UART_II_MODEM_CHG:         // 只支持串口0
				break;
		default:
				break;
	}
}
#endif

/**
* @brief  使用microLib的方法,使用printf
* @warningg 勾选 microLib
* @param  None
* @param  None
* @param  None
* @retval None
* @example 
**/

//int fputc(int ch, FILE *f)
//{
//	uint8_t data = ch;
//	PutData(&rb_t_usart3_send,&data,1);
//	UART3_INTCfg( ENABLE,RB_IER_THR_EMPTY );//使能中断发送
//  return ch;
//}

void log_hex(char *buf,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		debug_printf("%02X ",buf[i]);
	}
	debug_printf("\r\n");
}

                                                                                                                                                              //                                                                                                                                                                                                                                                             //
void LoRa_SendData(volatile unsigned char *pdate,volatile unsigned char *pmac,unsigned char lv8u_tx_length)
{      

	  unsigned char i=0; 
	  unsigned short data_sum=0; 
	  unsigned char date[30]={0};  
		                                                                                                                 
    date[i++] = 0x68;                                                                                                       //针头
    date[i++] = 0xAA;  
    date[i++] = 0x0C+lv8u_tx_length;                                                                                        //所发数据的长度
    date[i++] = 0x00; 
    date[i++] = pmac[0];                                                                                                    //针头
    date[i++] = pmac[1];  
    date[i++] = pmac[2];                                                                                                                    
    date[i++] = pmac[3];  
    date[i++] = pmac[4];                                                                                                                   
    date[i++] = pmac[5]; 
    for(i=0;i<lv8u_tx_length;i++)
       {
	      date[10+i] = pdate[i];
	      data_sum=date[10+i]+data_sum;
       }
    i=lv8u_tx_length+10;

    date[i++] = date[2]+date[3]+date[4]+date[5]+date[6]+date[7]+date[8]+date[9]+data_sum; 
    date[i++] = 0X16; 

		uart_send_byte_it(0, date, i);
    debug_printf("[usart0-->send_loar_data]: ");
	  log_hex((char*)date, i);//串口打印			 
}


void LORA_Init(void) 
{
  uart_send_byte_it(0, LORA_INIT, 12);
  debug_printf("LORA INIT!\r\n");
  DelayMs(1000);
  uart_send_byte_it(0, LORA_DOUBLE, 7);
  debug_printf("LORA DOUBLE!\r\n");
  DelayMs(1000);
  uart_send_byte_it(0, LORA_SAVE, 7);
  debug_printf("LORA SAVE!\r\n");
}
/**
 * @brief   串口数据处理
 * @param   None
 * @retval  None
 * @warning None
 * @example
 **/




