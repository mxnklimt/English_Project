#ifndef TCP_DATA_H_
#define TCP_DATA_H_

#ifndef TCP_DATA_C_//如果没有定义
#define TCP_DATA_Cx_ extern
#else
#define TCP_DATA_Cx_
#endif

#include "CH57x_common.h"

#define register_back        0xFF00
#define time_back            0xFF07
#define beat_back            0xFF08
#define Node_Query_updata    0xFF0C
#define batch_import         0xFF0D
#define batch_delete         0xFF0E

#define test_node            0x6000
#define Query_send           0x6001
#define updata_back          0x6002


/*config pin*/

void TCP_recv_date_handle(unsigned char *data, unsigned long length) ;
void tcp_recv_back(unsigned char sockeid, int socke_index, unsigned char *data,unsigned long length);
void tcp_connected_back(unsigned char socke_id, int socke_index) ;
void tcp_disconnected_back(unsigned char socke_id, int socke_index) ;
void dhcp_callback(void);
void dhcp_callback1(void);
#endif

