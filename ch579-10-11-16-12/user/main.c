#define MAIN_C_


#include "main.h"
#include "sys_tick.h"

#if 1 // �Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif


#define Task01_TASK_PRIO 4
#define Task01_STK_SIZE 256
TaskHandle_t Task01_Handler;
void StartTask01(void *pvParameters);

#define Task02_TASK_PRIO 3
#define Task02_STK_SIZE 256
TaskHandle_t Task02_Handler;
void StartTask02(void *pvParameters);

#define Task03_TASK_PRIO 2
#define Task03_STK_SIZE 256
TaskHandle_t Task03_Handler;
void StartTask03(void *pvParameters);

//#define Task04_TASK_PRIO 1
//#define Task04_STK_SIZE 256
//TaskHandle_t Task04_Handler;

void StartTask04(void *pvParameters);

void MX_FREERTOS_Init(void)
	{
  xTaskCreate((TaskFunction_t)StartTask01, (const char *)"StartTask01",
              (uint16_t)Task01_STK_SIZE, (void *)NULL,
              (UBaseType_t)Task01_TASK_PRIO, (TaskHandle_t *)&Task01_Handler);

  xTaskCreate((TaskFunction_t)StartTask02, (const char *)"StartTask02",
              (uint16_t)Task02_STK_SIZE, (void *)NULL,
              (UBaseType_t)Task02_TASK_PRIO, (TaskHandle_t *)&Task02_Handler);

  xTaskCreate((TaskFunction_t)StartTask03, (const char *)"StartTask03",
              (uint16_t)Task03_STK_SIZE, (void *)NULL,
              (UBaseType_t)Task03_TASK_PRIO, (TaskHandle_t *)&Task03_Handler);
//  xTaskCreate((TaskFunction_t)StartTask04, (const char *)"StartTask04",
//              (uint16_t)Task04_STK_SIZE, (void *)NULL,
//              (UBaseType_t)Task04_TASK_PRIO, (TaskHandle_t *)&Task04_Handler);

  vTaskStartScheduler(); // �����������
}




int main() 
{
  PWR_UnitModCfg(ENABLE, UNIT_SYS_PLL); // ʹ��PLL(����̫���ṩʱ��)
  DelayMs(3);
  GPIOB_ModeCfg(GPIO_Pin_0 | GPIO_Pin_1, GPIO_ModeOut_PP_20mA);
  key_init();
  uart_init();  // ��ʼ������
  net_init();   // ��ʼ����̫��
  time0_init(); // ��ʼ����ʱ��
	time1_init();
  time3_init();	
	LORA_Init();
  RTC_ClearITFlag(RTC_TMR_EVENT);			
  DHCP_UDP_TCP_LINK_Init() ;
	DelayMs(3);
  MX_FREERTOS_Init();
  debug_printf("start\r\n");	
	
	while(1)
	{
		
	}
}
   /* �ر����� */
void StartTask01(void *pvParameters) 
{
  for (;;) 
	{				
    net_dhcp_while(dhcp_callback);                // DHCP
    net_tcp_client_connect(&net_tcp_client);      // ���ӷ�����			     
		CH57xNET_MainTask(); /* CH57xNET��������������Ҫ����ѭ���в��ϵ��� */
    if (CH57xNET_QueryGlobalInt()) CH57xNET_HandleGlobalInt();  /* ��ѯ�¼���������¼�������ô����� */
		vTaskDelay(10);
  }
}

void StartTask02(void *pvParameters) 
{
 for (;;) 
	{
    uart_data();                                 //���������жϴ���
    vTaskDelay(10);
  }
}

void StartTask03(void *pvParameters)
{
 for(;;) 
	{	
    if(KEY_Scan()) key_count = 1;     //�����ж�		 9_22
    Lora_Link_Net();	 
    vTaskDelay(10);
  }
}
//void StartTask04(void *pvParameters) 
//{
// for (;;) 
//	{
// 
//    vTaskDelay(10);
//  }
//}


	

 



