#define RTC_C_

#include "rtc.h"

#if 1 //
#define debug_printf printf
#else
#define debug_printf(...)
#endif

RTC_TIME GET_TIME;
unsigned char count1=0;

//void test_30s(void)  //��Ϊ����ʱ���
//{
//  unsigned long i;
//  int a = 0;
//	unsigned char Date1[6] = {0x06,0, 0, 0, 0, 0};
//	unsigned char mac_address[6] = {0};		

//	StTime.tm_year = GET_TIME.Year;
//  StTime.tm_mon  = GET_TIME.Month;
//  StTime.tm_day  = GET_TIME.Date;
//  StTime.tm_hour = GET_TIME.Hours;
//  StTime.tm_min  = GET_TIME.Minutes;
//  StTime.tm_sec  = GET_TIME.Seconds;
//	i=GetUnixSecond(&StTime,timeZone);
//						 		 						 
//	Date1[2]=(i>>24);	 
//	Date1[3]=(i>>16);
//	Date1[4]=(i>>8);	 
//	Date1[5]=i;
//	debug_printf("Start ! MAX_NODE  = %d\r\n", (int)MAX_NODE);
//		
//	for(a = 0; a < MAX_NODE; a++)
//		{
//		  if(MAC_ARRAY[a][0] != 0xFF && MAC_ARRAY[a][0] != 0x00)
//		   {			
//				  Date1[1]=a+1;
//			    memcpy(mac_address, MAC_ARRAY[a], 6);		
//			    LoRa_SendData(Date1, mac_address, 6);
//				  DelayMs(1000);
//			 }
//		}
//	debug_printf("End!\r\n");	 						
//}

void RTC_Int( void )
{
	 LClk32K_Select( Clk32K_LSI);
   RTC_SetCycle32k( 2600000 );
   //RTC_InitTime( 2022, 9, 5, 14, 30, 10 );    //RTCʱ�ӳ�ʼ����ǰʱ��
	 RTC_TMRFunCfg( Period_4_S );               //RTC��ʱģʽ����
}

void RTC_Date( void )
{
	if( 0 != RTC_GetITFlag( RTC_TMR_EVENT ))  
	 {		 
		 RTC_ClearITFlag( RTC_TMR_EVENT );					
		 RTC_GetTime( &GET_TIME.Year, &GET_TIME.Month, &GET_TIME.Date, &GET_TIME.Hours, &GET_TIME.Minutes, &GET_TIME.Seconds ); 							 
		 if(GET_TIME.Old_Month != GET_TIME.Month)
			{ 
				if(15 == GET_TIME.Hours)
				 {
				   count1=count1+1;
					 if(1 == count1)
						{					     
						  count1 = 0; 
					//	bind_flag = 1;                               // 9.5号 要改
	            debug_printf("67xnjjxdsdzdhsj!\r\n");							 
						}							 									 
				   GET_TIME.Old_Date = GET_TIME.Date;	
				 }
		  }	 
	 					 
	 }
}


