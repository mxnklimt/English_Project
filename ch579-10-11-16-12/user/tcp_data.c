#define TCP_DATA_C_

#include "tcp_data.h"


#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif


/*接收消息回调*/
void tcp_recv_back(unsigned char sockeid, int socke_index, unsigned char *data,unsigned long length) 
{
   TCP_recv_date_handle(data, length);
}


/*连接上服务器*/
void tcp_connected_back(unsigned char socke_id, int socke_index) 
{
  debug_printf("#tcp_connected_back !!\n");
}


/*连接断开 或 未连接成功*/
void tcp_disconnected_back(unsigned char socke_id, int socke_index) 
{
  debug_printf("#tcp_disconnected_back !!\n");
  net_tcp_client_reconnect(&net_tcp_client); // 重新连接
}


// 每次dhcp成功会进入此函数,如果有需要初始化的在这里面初始化
void dhcp_callback(void) 
{
		debug_printf("net_tcp_client_creat_socket1111111111\r\n");
  net_tcp_client_close(&net_tcp_client); // 先尝试关闭(必须加)
  /*创建socket*/
	net_tcp_client_creat_socket(&net_tcp_client, tcp_connected_back,tcp_disconnected_back, tcp_recv_back);
	debug_printf("net_tcp_client_creat_socket\r\n");
	net_tcp_client_connect(&net_tcp_client); // 连接服务器
}


void dhcp_callback1(void)
{	
	
    net_tcp_client.socke_id = 255;  // 初始化该值为255(必须设置下),设置为255就可以
		CH57xNET_CreatUdpSocket(&SocketId);
    debug_printf("UDP Socket creat success! ...123456789\r\n");	
		main_len1 = 0;
}


void TCP_recv_date_handle(unsigned char *data, unsigned long length)
{
	uint16_t state = 0 ;
	state = data[11];
  state = (state << 8) | data[12];
  switch(state)         //按键状态处理
		 {
		   case register_back :      
		    {
          tcp_recv_server_register_back_data(data, length);
			    break;
        }
		   case time_back :	
        {		 
          tcp_recv_server_time_back_data(data, length);
			    break;
        }	
		   case beat_back :	
        {		 
          tcp_recv_server_beat_back_data(data, length);
			    break;
        }
		   case Node_Query_updata :	
        {		 
          tcp_recv_server_Node_Query_data(data, length); 
			    break;
        }	
		   case batch_import :      
		    {
          tcp_recv_server_batch_import_data(data, length);
			    break;
        }
		   case batch_delete :	
        {		 
          tcp_recv_server_batch_delete_data(data, length);
			    break;
        }	
		   case test_node :	
        {		 
          tcp_recv_server_test_node_data(data, length);
			    break;
        }
		   case Query_send :	
        {		 
          tcp_recv_server_Query_send_data(data, length); 
			    break;
        }	
		   case updata_back :	
        {		 
          tcp_recv_server_updata_back_data(data, length); 
			    break;
        }				
		   default :       
		    {
					debug_printf(" default:from server date  ");		
			    log_hex((char *)data, length); // 串口打印
			    break;
		    }
			}
}

