#ifndef TCP_DATA_HANDLE_H_
#define TCP_DATA_HANDLE_H_

#ifndef TCP_DATA_HANDLE_C_//如果没有定义
#define TCP_DATA_HANDLE_Cx_ extern
#else
#define TCP_DATA_HANDLE_Cx_
#endif

#include "CH57x_common.h"
#include <flashdb.h>

TCP_DATA_HANDLE_Cx_ unsigned char MACnet_set[6];
TCP_DATA_HANDLE_Cx_ unsigned char MAC_Record[150][6];                    // 缓存数据,全局通用
TCP_DATA_HANDLE_Cx_ unsigned short Msgld;



/*config pin*/
void tcp_recv_server_register_back_data(unsigned char *data, unsigned long length);
void tcp_recv_server_time_back_data(unsigned char *data, unsigned long length);	
void tcp_recv_server_beat_back_data(unsigned char *data, unsigned long length);
void tcp_recv_server_Node_Query_data(unsigned char *data, unsigned long length);
void tcp_recv_server_batch_import_data(unsigned char *data, unsigned long length)	;
void tcp_recv_server_batch_delete_data(unsigned char *data, unsigned long length);
void tcp_recv_server_test_node_data(unsigned char *data, unsigned long length);
void tcp_recv_server_Query_send_data(unsigned char *data, unsigned long length);
void tcp_recv_server_updata_back_data(unsigned char *data, unsigned long length);
#endif
