#define TCP_DATA_HANDLE_C_

#include "tcp_data_handle.h"
#include "db.h"
#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

unsigned char MACnet_set[6]={0};
unsigned char MAC_Record[150][6]={0};                    // 缓存数据,全局通用
unsigned short Msgld = 0;


//FF00   服务器注册回包
void tcp_recv_server_register_back_data(unsigned char *data, unsigned long length)
{
	 unsigned short xintiaobao = 0;
	
   debug_printf("from server recv register_back_data  ");		
   log_hex((char *)data, length); // 串口打印		
				
   xintiaobao |= (data[17] << 8);
   xintiaobao |= data[18];
	 Hub_GetTime_server();
}	


//FF07  从服务器获取时间回包
void tcp_recv_server_time_back_data(unsigned char *data, unsigned long length)
{
	 unsigned long shijianchuo = 0;
	
   debug_printf("from server recv time_back_data  ");		
	 log_hex((char *)data, length); // 串口打印
			 
   shijianchuo |= (data[17] << 24);
   shijianchuo |= (data[18] << 16);
   shijianchuo |= (data[19] << 8);
   shijianchuo |= data[20];

   StTime = GetDateTimeFromUnixSecond(shijianchuo, 1970);
   PrintTime(&StTime);

   GET_TIME.Old_Year = StTime.tm_year;
   GET_TIME.Old_Month = StTime.tm_mon;
   GET_TIME.Old_Date = StTime.tm_day;
   GET_TIME.Old_Hours = StTime.tm_hour;
   GET_TIME.Old_Minutes = StTime.tm_min;
   GET_TIME.Old_Seconds = StTime.tm_sec;

   RTC_Int(); // 初始化rtc实时时钟

   RTC_InitTime(StTime.tm_year, StTime.tm_mon, StTime.tm_day, StTime.tm_hour,StTime.tm_min, StTime.tm_sec); // RTC时钟初始化当前时间
   RTC_GetTime(&GET_TIME.Year, &GET_TIME.Month, &GET_TIME.Date,&GET_TIME.Hours, &GET_TIME.Minutes, &GET_TIME.Seconds);
	 db_init();
	 db_query_send(0) ;
}
//FF08   服务器心跳回包
void tcp_recv_server_beat_back_data(unsigned char *data, unsigned long length)
{
   debug_printf("from server recv beat_date  ");		
   log_hex((char *)data, length); // 串口打印	
}	

//FF0C   服务器查询hub的节点
void tcp_recv_server_Node_Query_data(unsigned char *data, unsigned long length)
{
   unsigned short Msgld = 0;
	
   debug_printf("from server recv Node_Query_date  ");		
   log_hex((char *)data, length); // 串口打印	
   Msgld = data[6];
   Msgld = (Msgld << 8) | data[7];
//   Node_Query(Msgld, 0XFF0C, Node_counter);

}	
//
//FF0D   服务器批量导入节点命令
void tcp_recv_server_batch_import_data(unsigned char *data, unsigned long length)
{  
   debug_printf("from server recv batch_import_data  ");		
   log_hex((char *)data, length); // 串口打印	
	
   Msgld = data[6];
   Msgld = (Msgld << 8) | data[7];
   Bind_Node_Number = data[15];
   Bind_Node_Number = (Bind_Node_Number << 8) | data[16];
   debug_printf("max-bindnode=%d", Bind_Node_Number);
   Bind_Node_Mac=Bind_Node_Number;
	
   memcpy(MAC_Record, &data[17], Bind_Node_Number*6);
	 debug_printf("[<--Hub_Bind_Node<--hub] len=%02d:  len = %02d :", Bind_Node_Number ,Bind_Node_Mac);
   log_hex((char *)MAC_Record, Bind_Node_Number*6);	 
   Record_import_flag = 1 ;	
	 node_auto_link();   //启动自动配网
	
}	
//FF0E  服务器批量删除节点命令
void tcp_recv_server_batch_delete_data(unsigned char *data, unsigned long length)
{

   debug_printf("from server recv batch_delete_data  ");		
   log_hex((char *)data, length); // 串口打印	
	
   Msgld = data[6];
   Msgld = (Msgld << 8) | data[7];
   Bind_Node_Number = data[15];
   Bind_Node_Number = (Bind_Node_Number << 8) | data[16];
   debug_printf("unbindnode=%d", Bind_Node_Number);

   Bind_Node_Mac=Bind_Node_Number;
   memcpy(MAC_Record, &data[17], Bind_Node_Number*6);   //批量删除成功操作                      
   Record_delete_flag = 1 ;
   GPIOB_ResetBits(GPIO_Pin_1); 	
   node_auto_link() ;  
	
}	

//6000  服务器下发测试命令
void tcp_recv_server_test_node_data(unsigned char *data, unsigned long length)
{
	 unsigned short Msgld = 0;
   unsigned char TcpMacNet[6] = {0, 0, 0, 0, 0, 0};	
	 
   debug_printf("from server recv test_node_data  ");		
   log_hex((char *)data, length); // 串口打印	
	
   Msgld = data[6];
   Msgld = (Msgld << 8) | data[7];
   data[17] = data[17] + 1; 
   memcpy(&TcpMacNet[0], &data[20], 6); // 将节点的MAC地址存入数组里边
   LoRa_SendData(&data[17], TcpMacNet, 1);
	 
   server_test_back_date(Msgld, 0x6000 ,IMPORT_DELETE_SUCCESS ) ;
}	

//6001  服务器下发查询存储的数据
void tcp_recv_server_Query_send_data(unsigned char *data, unsigned long length)
{	 
   debug_printf("from server recv Query_send_data  ");		
   log_hex((char *)data, length); // 串口打印	
	
   if(data[13] == 1)     
    { 
			db_query_send(1) ;
		}
   else if (data[13] == 0) 
    {
			db_query_send(0) ;
		}
	
}	

//6002  hub发送成功，服务器回包数据
void tcp_recv_server_updata_back_data(unsigned char *data, unsigned long length)
{	 
   debug_printf("from server recv updata_back_data  ");		
   log_hex((char *)data, length); // 串口打印	
	
/*	
     shijianchuo |= (data[15] << 24);
     shijianchuo |= (data[16] << 16);
     shijianchuo |= (data[17] << 8);
     shijianchuo |= data[18];

     StTime = GetDateTimeFromUnixSecond(shijianchuo, 1970);
     PrintTime(&StTime);

    //			GET_TIME.Old_Year = StTime.tm_year; //3月31号 调到4月1号
    // 会一直判断天数 			GET_TIME.Old_Month = StTime.tm_mon;
    //			GET_TIME.Old_Date = StTime.tm_day;
    //			GET_TIME.Old_Hours = StTime.tm_hour;
    //			GET_TIME.Old_Minutes = StTime.tm_min;
    //			GET_TIME.Old_Seconds =StTime.tm_sec;

     RTC_Int(); // 初始化rtc实时时钟
     RTC_InitTime(StTime.tm_year, StTime.tm_mon, StTime.tm_day, StTime.tm_hour,StTime.tm_min, StTime.tm_sec); // RTC时钟初始化当前时间	
		 */
}	


