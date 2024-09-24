#define net_tcp_client_c_

#include "net_tcp_client.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

net_tcp_client_struct net_tcp_client;     // tcp_client����

/**
* @brief   �����ͻ���
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_creat_socket(net_tcp_client_struct *net_tcp_client, 
	net_tcp_client_back connected_back, //���ӳɹ��ص�����
	net_tcp_client_back disconnected_back ,//���ӶϿ��ص�����
	net_tcp_client_recv_back received_back)//�������ݻص�����
{
	unsigned char state;                                                             
	SOCK_INF TmpSocketInf;                                                       /* ������ʱsocket���� */
	net_tcp_client->connected_state=0;
	memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
	memcpy((void *)TmpSocketInf.IPAddr,net_tcp_client->ip_addr_remote,4);        /* ����Ŀ��IP��ַ */
	TmpSocketInf.DesPort = net_tcp_client->port_remote;                     
	TmpSocketInf.SourPort = net_tcp_client->port_local;                          /* ����Դ�˿� */
	TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* ����socekt���� */
	TmpSocketInf.RecvStartPoint = (unsigned long)net_tcp_client->socket_recv_buf;                        /* ���ý��ջ������Ľ��ջ����� */
	TmpSocketInf.RecvBufLen = net_tcp_client_socket_recv_buf_len ;                                     /* ���ý��ջ������Ľ��ճ��� */
	state = CH57xNET_SocketCreat(&(net_tcp_client->socke_id),&TmpSocketInf);         /* ����socket�������ص�socket����������SocketId�� */
	if(state!=CH57xNET_ERR_SUCCESS) /* ������ */
	{
		debug_printf("net_tcp_client_creat_socket_error: %02X\r\n", (unsigned short)state);
		return state;
	}
	debug_printf("net_tcp_client_creat_socket_id: %d\r\n", net_tcp_client->socke_id);
	if(net_tcp_client->keeplive_enable==1){
		CH57xNET_SocketSetKeepLive( net_tcp_client->socke_id, 1 ); /* ����socket��KEEPLIVE���� */
	}
	if(connected_back != NULL) net_tcp_client->connected_back = connected_back;
	if(disconnected_back != NULL) net_tcp_client->disconnected_back = disconnected_back;
	if(received_back != NULL) net_tcp_client->received_back = received_back;
	return 0;
//	i = CH57xNET_SocketConnect(SocketId4);                                       /* TCP���� */
//	mStopIfError(i);                                                             /* ������ */
//	i = CH57xNET_SetSocketTTL( SocketId4,10 );
//	mStopIfError(i);                                                             /* ������ */
}



void net_tcp_client_connect(net_tcp_client_struct *net_tcp_client)
{
	unsigned char state;
	if(net_tcp_client->connected_state==0)//δִ������
	{
		debug_printf("net_tcp_client_connect......\r\n");
		state = CH57xNET_SocketConnect(net_tcp_client->socke_id);
		if(state!=CH57xNET_ERR_SUCCESS) /* ������ */
		{
			debug_printf("net_tcp_client_connect_error: %02X\r\n", (unsigned short)state);
		}
		net_tcp_client->connected_state = 1;
	}
}


/**
* @brief   ��������
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_send(net_tcp_client_struct *net_tcp_client, unsigned char *buf, unsigned long len)
{
	unsigned long length = len;
	unsigned long totallen = length;
	if(net_tcp_client->connected_state==2)//������
	{
		while(1)
		{
			 length = totallen;
			 CH57xNET_SocketSend(net_tcp_client->socke_id, buf, &length);         /* ��MyBuf�е����ݷ��� */
			 totallen -= length;                                                     /* ���ܳ��ȼ�ȥ�Լ�������ϵĳ��� */
			 buf += length;                                                            /* ��������ָ��ƫ��*/
			 if(totallen)continue;                                                /* �������δ������ϣ����������*/
			 break;                                                               /* ������ϣ��˳� */
		}
	}
	else{
		return 5;
	}
	return 0;
}


/**
* @brief   �ر�socket
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_close(net_tcp_client_struct *net_tcp_client)
{
	int socke_id = net_tcp_client->socke_id;
	net_tcp_client->socke_id=255;
	return CH57xNET_SocketClose( socke_id, TCP_CLOSE_NORMAL );
}

/**
* @brief   ��������TCP
* @param   socketid
* @param   buf ���ݵ�ַ
* @param   len ���ݳ���
* @retval  0:�ɹ�; others:ʧ��
* @warning None
* @example 
**/
char net_tcp_client_reconnect(net_tcp_client_struct *net_tcp_client)
{
	CH57xNET_SocketClose( net_tcp_client->socke_id, TCP_CLOSE_NORMAL );
	net_tcp_client_creat_socket(net_tcp_client, NULL, NULL, NULL);
	net_tcp_client_connect(net_tcp_client);//���ӷ�����
	return 0;
}


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
void net_tcp_client_data(net_tcp_client_struct *net_tcp_client,unsigned char sockeid, unsigned char initstat)
{
	unsigned long len;

	if(sockeid == net_tcp_client->socke_id)
	{
		if(initstat & SINT_STAT_RECV)//���յ�����
    {
			len = CH57xNET_SocketRecvLen(sockeid, NULL);//��ȡ���ݸ���
			if (len > 0)
			{
				CH57xNET_SocketRecv(sockeid, net_tcp_client->client_recv_buf, &len);//��ȡ����
				if(net_tcp_client->received_back != NULL)
				{
					net_tcp_client->received_back(net_tcp_client->socke_id, net_tcp_client->socke_index, net_tcp_client->client_recv_buf, len);
				}
			}
    }
		if(initstat & SINT_STAT_CONNECT)/* TCP����*/
		{
			net_tcp_client->connected_state=2;
			if(net_tcp_client->connected_back != NULL)
			 {
				 net_tcp_client->connected_back(net_tcp_client->socke_id, net_tcp_client->socke_index);
			 }
			debug_printf("tcp connect cuccess:%d-%d\r\n",net_tcp_client->socke_id, net_tcp_client->socke_index);			   		 
      Hub_register_server();
//      register_date();			 
		}
		if(initstat & SINT_STAT_DISCONNECT)/* TCP�Ͽ� */
		{/* �������жϣ�CH579���ڲ��Ὣ��socket�������Ϊ�ر�,��Ҫ���´���*/
			net_tcp_client->connected_state=0;
			
//			net_tcp_client_creat_socket(net_tcp_client, NULL,NULL,NULL);//���´���
			debug_printf("tcp disconnected:%d-%d\r\n",net_tcp_client->socke_id, net_tcp_client->socke_index);
			net_tcp_client->socke_id=255;
			if(net_tcp_client->disconnected_back != NULL)
			{
				net_tcp_client->disconnected_back(net_tcp_client->socke_id, net_tcp_client->socke_index);
			}
		}
		if(initstat & SINT_STAT_TIM_OUT)/* TCP��ʱ�ж� */
		{/* �������жϣ�CH579���ڲ��Ὣ��socket�������Ϊ�ر�,��Ҫ���´���*/
			net_tcp_client->connected_state=0;
			
//			net_tcp_client_creat_socket(net_tcp_client, NULL,NULL,NULL);//���´���
			debug_printf("tcp disconnected out:%d-%d\r\n",net_tcp_client->socke_id, net_tcp_client->socke_index);
			net_tcp_client->socke_id=255;
			if(net_tcp_client->disconnected_back != NULL)
			{
				net_tcp_client->disconnected_back(net_tcp_client->socke_id, net_tcp_client->socke_index);
			}
		}
	}
}


/*******************************************************************************
 * Function Name  : CH57xNET_HandleGloableInt
 * Description    : �¼�������
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_HandleGlobalInt(void) {
  UINT8 initstat;
  UINT8 i;
  UINT8 socketinit;
  initstat = CH57xNET_GetGlobalInt(); /* ��ȫ���ж�״̬����� */

  if(initstat & GINT_STAT_UNREACH)  /* ���ɴ��ж� */
   {  
	   debug_printf("UnreachCode ��%ld\n",CH57xInf.UnreachCode); /* �鿴���ɴ���� */
     debug_printf("UnreachProto ��%ld\n",CH57xInf.UnreachProto); /* �鿴���ɴ�Э������ */
     debug_printf("UnreachPort ��%ld\n",CH57xInf.UnreachPort); /* ��ѯ���ɴ�˿� */
   }
  if(initstat & GINT_STAT_IP_CONFLI) { /* IP��ͻ�ж� */
   }
  if(initstat & GINT_STAT_PHY_CHANGE)  /* PHY�ı��ж� */
   { 
		 i = CH57xNET_GetPHYStatus();         /* ��ȡPHY״̬ */
     debug_printf("GINT_STAT_PHY_CHANGE %02x\n", i);
   }
  if(initstat & GINT_STAT_SOCKET) /* Socket�ж� */
   {  
		for(i = 0; i < CH57xNET_MAX_SOCKET_NUM; i++) 
		  {
        socketinit = CH57xNET_GetSocketInt(i); /* ��socket�жϲ����� */
        if(socketinit) 
				 {                      // ���¼���Ҫ����
//           debug_printf("Socket Interupt!\r\n");
           net_tcp_client_data(&net_tcp_client, i, socketinit); // ����TCP����
				   net_udp_socket_data(i, socketinit);		
         }
      }
   }
}



void DHCP_UDP_TCP_LINK_Init(void) 
{
	
while(!main_len1)
	  {	
	    net_dhcp_while(NULL);
	    CH57xNET_MainTask(); 
	  }
  dhcp_callback1();
	while(!UDP_Received_Flag)
	  {
	    CH57xNET_SocketUdpSendTo(SocketId, broadcast_payload, &broadcast_len,broadcast_ip, broadcast_port);debug_printf("UDP Send Success \r\n");
	    debug_printf("UDP Send Success \r\n");
	    CH57xNET_MainTask(); 
      if (CH57xNET_QueryGlobalInt()) CH57xNET_HandleGlobalInt();
		  DelayMs(500);
	  }
	 UDP_Received_Flag = 0 ;
	 debug_printf("UDP Received \r\n");
   net_tcp_client.ip_addr_remote[0] = ip_addr_from_udp[0];           /*���÷�������IP��ַ*/
   net_tcp_client.ip_addr_remote[1] = ip_addr_from_udp[1];
   net_tcp_client.ip_addr_remote[2] = ip_addr_from_udp[2];
   net_tcp_client.ip_addr_remote[3] = ip_addr_from_udp[3];
           /*���ö˿ں�*/
   net_tcp_client.port_remote = port_from_udp; // Ҫ���ӵķ������˿ں�
   net_tcp_client.port_local = 2002;           // ���ض˿ں� ��ʽ��Ҫ���
   debug_printf("ip_addr_remote : %d.%d.%d.%d  port_remote : %d\r\n",net_tcp_client.ip_addr_remote[0], net_tcp_client.ip_addr_remote[1],net_tcp_client.ip_addr_remote[2], net_tcp_client.ip_addr_remote[3],net_tcp_client.port_remote);

   net_tcp_client.socke_index = 0; // ��ÿ���ͻ�������һ����ʶ(���Բ�����)
   net_tcp_client.socke_id = 255;  // ��ʼ����ֵΪ255(����������),����Ϊ255�Ϳ���		
   debug_printf("Destination address:%d.%d.%d.%d port:%d\r\n",net_tcp_client.ip_addr_remote[0],net_tcp_client.ip_addr_remote[1],net_tcp_client.ip_addr_remote[2],net_tcp_client.ip_addr_remote[3], net_tcp_client.port_remote);	
   //net_tcp_client_creat_socket(&net_tcp_client, tcp_connected_back,tcp_disconnected_back, tcp_recv_back);
   //net_tcp_client_connect(&net_tcp_client);    

	 dhcp_callback();
	
}






