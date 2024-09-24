#define UART_C_

#include "uart.h"
#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

static unsigned char LORA_INIT[12]  = {0x68, 0xAA, 0x0C, 0x14, 0x13, 0x24,0x35, 0x46, 0x57, 0x68, 0x91, 0x16};
static unsigned char LORA_DOUBLE[7] = {0x68, 0xAA, 0X07, 0X0F, 0X01, 0X17, 0X16};
static unsigned char LORA_SAVE[7]   = {0x68, 0xAA, 0X07, 0X13, 0X01, 0X1B, 0X16};

#if UART1_ENABLE
/****************************����1****************************/
//���ջ��ζ���
rb_t rb_t_usart1_read;
//���ζ��л�������
unsigned char rb_t_usart1_read_buff[rb_t_usart1_read_buff_len];
//�ӻ��濽������ʹ��
unsigned char usart1_read_buff_copy[rb_t_usart1_read_buff_len];
//���ռ���
int usart1_read_count=0;
int usart1_read_count_copy=0;
//���յ�һ֡���ݱ�־
unsigned char usart1_idle_flag=0;


//���ͻ��ζ���
rb_t rb_t_usart1_send;
//���ζ��л�������
unsigned char rb_t_usart1_send_buff[rb_t_usart1_send_buff_len];
//������ȡ���ζ���1���ֽ�
unsigned char rb_t_usart1_send_byte;
#endif



#if UART0_ENABLE
/****************************����0****************************/
//���ջ��ζ���
rb_t rb_t_usart0_read;
//���ζ��л�������
unsigned char rb_t_usart0_read_buff[rb_t_usart0_read_buff_len];
//�ӻ��濽������ʹ��
unsigned char usart0_read_buff_copy[rb_t_usart0_read_buff_len];
//���ռ���
int usart0_read_count=0;
int usart0_read_count_copy=0;
//���յ�һ֡���ݱ�־
unsigned char usart0_idle_flag=0;

//���ͻ��ζ���
rb_t rb_t_usart0_send;
//���ζ��л�������
unsigned char rb_t_usart0_send_buff[rb_t_usart0_send_buff_len];
//������ȡ���ζ���1���ֽ�
unsigned char rb_t_usart0_send_byte;
#endif

#if UART3_ENABLE
/****************************����3****************************/
//���ջ��ζ���
rb_t rb_t_usart3_read;
//���ζ��л�������
unsigned char rb_t_usart3_read_buff[rb_t_usart3_read_buff_len];
//�ӻ��濽������ʹ��
unsigned char usart3_read_buff_copy[rb_t_usart3_read_buff_len];
//���ռ���
int usart3_read_count=0;
int usart3_read_count_copy=0;
//���յ�һ֡���ݱ�־
unsigned char usart3_idle_flag=0;

//���ͻ��ζ���
rb_t rb_t_usart3_send;
//���ζ��л�������
unsigned char rb_t_usart3_send_buff[rb_t_usart3_send_buff_len];
//������ȡ���ζ���1���ֽ�
unsigned char rb_t_usart3_send_byte;
#endif 

void uart_init(void)
{
#if UART1_ENABLE
	/*���ô���1*/
	//�������ζ���(���ڴ���1��������)
	rbCreate(&rb_t_usart1_read,rb_t_usart1_read_buff,rb_t_usart1_read_buff_len);
	//�������ζ���(���ڴ���1��������)
	rbCreate(&rb_t_usart1_send,rb_t_usart1_send_buff,rb_t_usart1_send_buff_len);
	
	GPIOA_SetBits(GPIO_Pin_9);
	GPIOA_ModeCfg(GPIO_Pin_8, GPIO_ModeIN_PU);     // RXD-������������
	GPIOA_ModeCfg(GPIO_Pin_9, GPIO_ModeOut_PP_5mA);// TXD-�������������ע������IO������ߵ�ƽ
	UART1_DefInit();
	/*ʹ�ܽ����ж�,���մ����ж�*/
	UART1_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	//	NVIC_SetPriority(TMR0_IRQn,18);//�����ж����ȼ�(Ҳ���Բ�����,Ĭ��14)
	NVIC_EnableIRQ( UART1_IRQn );
#endif 
	
#if UART0_ENABLE	
	/*���ô���0*/
	//�������ζ���(���ڴ��ڽ�������)
	rbCreate(&rb_t_usart0_read, rb_t_usart0_read_buff, rb_t_usart0_read_buff_len);
	//�������ζ���(���ڴ��ڷ�������)
	rbCreate(&rb_t_usart0_send, rb_t_usart0_send_buff, rb_t_usart0_send_buff_len);
	
	GPIOPinRemap(ENABLE,RB_PIN_UART0); //�ض�������
	GPIOA_SetBits(GPIO_Pin_14);//TX����ߵ�ƽ
	GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);     // RXD-������������
	GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA);// TXD-�������������ע������IO������ߵ�ƽ
	UART0_DefInit();
	/*ʹ�ܽ����ж�,���մ����ж�*/
	UART0_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	//	NVIC_SetPriority(TMR0_IRQn,18);//�����ж����ȼ�(Ҳ���Բ�����,Ĭ��14)
	NVIC_EnableIRQ( UART0_IRQn );
#endif

#if UART3_ENABLE
	/*���ô���3*/
	//�������ζ���(���ڴ��ڽ�������)
	rbCreate(&rb_t_usart3_read, rb_t_usart3_read_buff, rb_t_usart3_read_buff_len);
	//�������ζ���(���ڴ��ڷ�������)
	rbCreate(&rb_t_usart3_send, rb_t_usart3_send_buff, rb_t_usart3_send_buff_len);
	
	GPIOPinRemap(ENABLE,RB_PIN_UART3); //�ض�������
	GPIOB_SetBits(GPIO_Pin_21);
	GPIOB_ModeCfg(GPIO_Pin_20, GPIO_ModeIN_PU);     // RXD-������������
	GPIOB_ModeCfg(GPIO_Pin_21, GPIO_ModeOut_PP_5mA);// TXD-�������������ע������IO������ߵ�ƽ
	UART3_DefInit();
	/*ʹ�ܽ����ж�,���մ����ж�*/
	UART3_INTCfg( ENABLE, RB_IER_RECV_RDY|RB_IER_LINE_STAT );
	NVIC_EnableIRQ( UART3_IRQn );
#endif
}



/**
* @brief  ���ڷ����ַ�������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
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
* @brief  ���ڷ���һ���ֽ�
* @param  *c:���͵�����ָ��  cnt:���ݸ���
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
* @brief  �����жϷ�ʽ��������
* @param  *c:���͵�����ָ��  cnt:���ݸ���
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
			UART0_INTCfg( ENABLE,RB_IER_THR_EMPTY );//ʹ���жϷ���
			break;
		case 1: 
			#if UART1_ENABLE
			PutData(&rb_t_usart1_send,c,cnt);
			UART1_INTCfg( ENABLE,RB_IER_THR_EMPTY );//ʹ���жϷ���
			#endif
			break;
		case 2: 
			#if UART2_ENABLE
			PutData(&rb_t_usart2_send,c,cnt);
			UART2_INTCfg( ENABLE,RB_IER_THR_EMPTY );//ʹ���жϷ���
			#endif			
			break;
		case 3:  
			PutData(&rb_t_usart3_send,c,cnt);
			UART3_INTCfg( ENABLE,RB_IER_THR_EMPTY );//ʹ���жϷ���
			break;
	}
}

#if UART0_ENABLE
/*�����ж�*/
void UART0_IRQHandler(void)
{
	char data;
	switch( UART0_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // ��·״̬����
				UART0_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // ���ݴﵽ���ô�����
				data = UART0_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart0_read,&data,1);
				usart0_read_count++;
				break;
		case UART_II_RECV_TOUT:         // ���ճ�ʱ����ʱһ֡���ݽ������
				data = UART0_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart0_read,&data,1);
				usart0_read_count++;
				usart0_read_count_copy = usart0_read_count;
				usart0_read_count=0;
				usart0_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // ���ͻ������գ��ɼ�������
				if(rbCanRead(&rb_t_usart0_send)>0)//�����������ݸ�������0
				{
					rbRead(&rb_t_usart0_send,&rb_t_usart0_send_byte,1);
					UART0_SendByte(rb_t_usart0_send_byte);
				}
				else
				{
					UART0_INTCfg( DISABLE,RB_IER_THR_EMPTY );//�ر��жϷ���
				}
				break;
		case UART_II_MODEM_CHG:         // ֻ֧�ִ���0
				break;
		default:
				break;
	}
}
#endif

#if UART2_ENABLE
/*�����ж�*/
void UART2_IRQHandler(void)
{
	char data;
	switch( UART2_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // ��·״̬����
				UART2_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // ���ݴﵽ���ô�����
				data = UART2_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart2_read,&data,1);
				usart2_read_count++;
				break;
		case UART_II_RECV_TOUT:         // ���ճ�ʱ����ʱһ֡���ݽ������
				data = UART2_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart2_read,&data,1);
				usart2_read_count++;
				usart2_read_count_copy = usart2_read_count;
				usart2_read_count=0;
				usart2_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // ���ͻ������գ��ɼ�������
				if(rbCanRead(&rb_t_usart2_send)>0)//�����������ݸ�������0
				{
					rbRead(&rb_t_usart2_send,&rb_t_usart2_send_byte,1);
					UART2_SendByte(rb_t_usart2_send_byte);
				}
				else
				{
					UART2_INTCfg( DISABLE,RB_IER_THR_EMPTY );//�ر��жϷ���
				}
				break;
		case UART_II_MODEM_CHG:         // ֻ֧�ִ���0
				break;
		default:
				break;
	}
}
#endif

#if UART1_ENABLE
/*�����ж�*/
void UART1_IRQHandler(void)
{
	char data;
	switch( UART1_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // ��·״̬����
				UART1_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // ���ݴﵽ���ô�����
				data = UART1_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart1_read,&data,1);
				usart1_read_count++;
				break;
		case UART_II_RECV_TOUT:         // ���ճ�ʱ����ʱһ֡���ݽ������
				data = UART1_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart1_read,&data,1);
				usart1_read_count++;
				usart1_read_count_copy = usart1_read_count;
				usart1_read_count=0;
				usart1_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // ���ͻ������գ��ɼ�������
				if(rbCanRead(&rb_t_usart1_send)>0)//�����������ݸ�������0
				{
					rbRead(&rb_t_usart1_send,&rb_t_usart1_send_byte,1);
					UART1_SendByte(rb_t_usart1_send_byte);
				}
				else
				{
					UART1_INTCfg( DISABLE,RB_IER_THR_EMPTY );//�ر��жϷ���
				}
				break;
		case UART_II_MODEM_CHG:         // ֻ֧�ִ���0
				break;
		default:
				break;
	}
}
#endif

#if UART3_ENABLE
/*�����ж�*/
void UART3_IRQHandler(void)
{
	char data;
	switch( UART3_GetITFlag() )
	{
		case UART_II_LINE_STAT:          // ��·״̬����
				UART3_GetLinSTA();
				break;
		case UART_II_RECV_RDY:          // ���ݴﵽ���ô�����
				data = UART3_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart3_read,&data,1);
				usart3_read_count++;
				break;
		case UART_II_RECV_TOUT:         // ���ճ�ʱ����ʱһ֡���ݽ������
				data = UART3_RecvByte();//��ȡ��������
				/*�������ݴ洢������*/
				PutData(&rb_t_usart3_read,&data,1);
				usart3_read_count++;
				usart3_read_count_copy = usart3_read_count;
				usart3_read_count=0;
				usart3_idle_flag=1;
				break;
		case UART_II_THR_EMPTY:         // ���ͻ������գ��ɼ�������
				if(rbCanRead(&rb_t_usart3_send)>0)//�����������ݸ�������0
				{
					rbRead(&rb_t_usart3_send,&rb_t_usart3_send_byte,1);
					UART3_SendByte(rb_t_usart3_send_byte);
				}
				else
				{
					UART3_INTCfg( DISABLE,RB_IER_THR_EMPTY );//�ر��жϷ���
				}
				break;
		case UART_II_MODEM_CHG:         // ֻ֧�ִ���0
				break;
		default:
				break;
	}
}
#endif

/**
* @brief  ʹ��microLib�ķ���,ʹ��printf
* @warningg ��ѡ microLib
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
//	UART3_INTCfg( ENABLE,RB_IER_THR_EMPTY );//ʹ���жϷ���
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
		                                                                                                                 
    date[i++] = 0x68;                                                                                                       //��ͷ
    date[i++] = 0xAA;  
    date[i++] = 0x0C+lv8u_tx_length;                                                                                        //�������ݵĳ���
    date[i++] = 0x00; 
    date[i++] = pmac[0];                                                                                                    //��ͷ
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
	  log_hex((char*)date, i);//���ڴ�ӡ			 
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
 * @brief   �������ݴ���
 * @param   None
 * @retval  None
 * @warning None
 * @example
 **/




