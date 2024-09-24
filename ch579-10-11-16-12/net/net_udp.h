#ifndef net_udp_h
#define net_udp_h

#ifndef net_udp_c//�0�6�0�4�0�1�0�4�0�1�0�3�0�7�0�4�0�9���0�6�0�2
#define net_udp_cx extern
#else
#define net_udp_cx
#endif

#include "CH57x_common.h"


net_udp_cx char UDP_Received_Flag;        // ȫ��ͨ�ñ���
net_udp_cx char ip_addr_from_udp[4];      // ȫ��ͨ�ñ���
net_udp_cx unsigned short port_from_udp;  // ȫ��ͨ�ñ���
net_udp_cx UINT8 broadcast_ip[4] ;        // ȫ��ͨ�ñ���
net_udp_cx UINT16 broadcast_port ;        // ȫ��ͨ�ñ���
net_udp_cx UINT8 broadcast_payload[18] ;  // ȫ��ͨ�ñ���
net_udp_cx UINT32 broadcast_len;          // ȫ��ͨ�ñ���
net_udp_cx UINT8 SocketId;    

void CH57xNET_HandleSockInt(UINT8 sockeid,UINT8 initstat);
void CH57xNET_CreatUdpSocket(UINT8 *SocketId);
void net_udp_socket_data(unsigned char sockeid, unsigned char initstat);
#endif
