#ifndef net_tcp_client_h_
#define net_tcp_client_h_

#ifndef net_tcp_client_c_
#define net_tcp_client_cx_ extern
#else
#define net_tcp_client_cx_
#endif

#include "CH57x_common.h"
#include "CH57xNET.H"


typedef void(*net_tcp_client_back)(unsigned char socke_id, int socke_index);
typedef void(*net_tcp_client_recv_back)(unsigned char sockeid, int socke_index, unsigned char* data, unsigned long length);

#define net_tcp_client_socket_recv_buf_len 536
typedef struct net_tcp_client_struct{
	unsigned char  ip_addr_remote[4];//Ŀ�ĵ�ַ
	unsigned int port_remote;//Ŀ�Ķ˿ں�
	unsigned int port_local;//���ض˿ں�
	unsigned char keeplive_enable;//�Ƿ�����������
	
	net_tcp_client_back connected_back;//���ӳɹ��ص�����
	net_tcp_client_back disconnected_back;//���ӶϿ��ص�����
	net_tcp_client_recv_back received_back;//�������ݻص�����
	unsigned char socke_id;//��¼sockeid
	unsigned char socke_index;
	unsigned char connected_state;//����״̬ 0:δ���� 1:������  2:������
	unsigned char socket_recv_buf[net_tcp_client_socket_recv_buf_len];
	unsigned char client_recv_buf[net_tcp_client_socket_recv_buf_len];
}net_tcp_client_struct;

net_tcp_client_cx_ net_tcp_client_struct net_tcp_client;          // tcp_client����


char net_tcp_client_creat_socket(net_tcp_client_struct *net_tcp_client, 
	net_tcp_client_back connected_back, //���ӳɹ��ص�����
	net_tcp_client_back disconnected_back ,//���ӶϿ��ص�����
	net_tcp_client_recv_back received_back);//�������ݻص�����


void net_tcp_client_connect(net_tcp_client_struct *net_tcp_client);

/**
* @brief   ���ݽ�������
* @param   sockeid
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void net_tcp_client_data(net_tcp_client_struct *net_tcp_client,unsigned char sockeid, unsigned char initstat);

/**
* @brief   ��������
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_send(net_tcp_client_struct *net_tcp_client, unsigned char *buf, unsigned long len);

/**
* @brief   �ر�socket
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_close(net_tcp_client_struct *net_tcp_client);

/**
* @brief   ��������TCP
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_reconnect(net_tcp_client_struct *net_tcp_client);

void CH57xNET_HandleGlobalInt(void) ;
void DHCP_UDP_TCP_LINK_Init(void) ;
#endif

