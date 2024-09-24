#define net_tcp_client_c_

#include "net_tcp_client.h"

#if 1 //是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif

net_tcp_client_struct net_tcp_client;     // tcp_client变量

/**
* @brief   创建客户端
* @param   socketid
* @param   buf 数据地址
* @param   len 数据长度
* @retval  0:成功; others:失败
* @warning None
* @example 
**/
char net_tcp_client_creat_socket(net_tcp_client_struct *net_tcp_client, 
	net_tcp_client_back connected_back, //连接成功回调函数
	net_tcp_client_back disconnected_back ,//连接断开回调函数
	net_tcp_client_recv_back received_back)//接收数据回调函数
{
	unsigned char state;                                                             
	SOCK_INF TmpSocketInf;                                                       /* 创建临时socket变量 */
	net_tcp_client->connected_state=0;
	memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* 库内部会将此变量复制，所以最好将临时变量先全部清零 */
	memcpy((void *)TmpSocketInf.IPAddr,net_tcp_client->ip_addr_remote,4);        /* 设置目的IP地址 */
	TmpSocketInf.DesPort = net_tcp_client->port_remote;                     
	TmpSocketInf.SourPort = net_tcp_client->port_local;                          /* 设置源端口 */
	TmpSocketInf.ProtoType = PROTO_TYPE_TCP;                                     /* 设置socekt类型 */
	TmpSocketInf.RecvStartPoint = (unsigned long)net_tcp_client->socket_recv_buf;                        /* 设置接收缓冲区的接收缓冲区 */
	TmpSocketInf.RecvBufLen = net_tcp_client_socket_recv_buf_len ;                                     /* 设置接收缓冲区的接收长度 */
	state = CH57xNET_SocketCreat(&(net_tcp_client->socke_id),&TmpSocketInf);         /* 创建socket，将返回的socket索引保存在SocketId中 */
	if(state!=CH57xNET_ERR_SUCCESS) /* 检查错误 */
	{
		debug_printf("net_tcp_client_creat_socket_error: %02X\r\n", (unsigned short)state);
		return state;
	}
	debug_printf("net_tcp_client_creat_socket_id: %d\r\n", net_tcp_client->socke_id);
	if(net_tcp_client->keeplive_enable==1){
		CH57xNET_SocketSetKeepLive( net_tcp_client->socke_id, 1 ); /* 开启socket的KEEPLIVE功能 */
	}
	if(connected_back != NULL) net_tcp_client->connected_back = connected_back;
	if(disconnected_back != NULL) net_tcp_client->disconnected_back = disconnected_back;
	if(received_back != NULL) net_tcp_client->received_back = received_back;
	return 0;
//	i = CH57xNET_SocketConnect(SocketId4);                                       /* TCP连接 */
//	mStopIfError(i);                                                             /* 检查错误 */
//	i = CH57xNET_SetSocketTTL( SocketId4,10 );
//	mStopIfError(i);                                                             /* 检查错误 */
}



void net_tcp_client_connect(net_tcp_client_struct *net_tcp_client)
{
	unsigned char state;
	if(net_tcp_client->connected_state==0)//未执行连接
	{
		debug_printf("net_tcp_client_connect......\r\n");
		state = CH57xNET_SocketConnect(net_tcp_client->socke_id);
		if(state!=CH57xNET_ERR_SUCCESS) /* 检查错误 */
		{
			debug_printf("net_tcp_client_connect_error: %02X\r\n", (unsigned short)state);
		}
		net_tcp_client->connected_state = 1;
	}
}


/**
* @brief   发送数据
* @param   socketid
* @param   buf 数据地址
* @param   len 数据长度
* @retval  0:成功; others:失败
* @warning None
* @example 
**/
char net_tcp_client_send(net_tcp_client_struct *net_tcp_client, unsigned char *buf, unsigned long len)
{
	unsigned long length = len;
	unsigned long totallen = length;
	if(net_tcp_client->connected_state==2)//连接中
	{
		while(1)
		{
			 length = totallen;
			 CH57xNET_SocketSend(net_tcp_client->socke_id, buf, &length);         /* 将MyBuf中的数据发送 */
			 totallen -= length;                                                     /* 将总长度减去以及发送完毕的长度 */
			 buf += length;                                                            /* 将缓冲区指针偏移*/
			 if(totallen)continue;                                                /* 如果数据未发送完毕，则继续发送*/
			 break;                                                               /* 发送完毕，退出 */
		}
	}
	else{
		return 5;
	}
	return 0;
}


/**
* @brief   关闭socket
* @param   socketid
* @param   buf 数据地址
* @param   len 数据长度
* @retval  0:成功; others:失败
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
* @brief   重新连接TCP
* @param   socketid
* @param   buf 数据地址
* @param   len 数据长度
* @retval  0:成功; others:失败
* @warning None
* @example 
**/
char net_tcp_client_reconnect(net_tcp_client_struct *net_tcp_client)
{
	CH57xNET_SocketClose( net_tcp_client->socke_id, TCP_CLOSE_NORMAL );
	net_tcp_client_creat_socket(net_tcp_client, NULL, NULL, NULL);
	net_tcp_client_connect(net_tcp_client);//连接服务器
	return 0;
}


/**
* @brief   数据解析函数
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
		if(initstat & SINT_STAT_RECV)//接收到数据
    {
			len = CH57xNET_SocketRecvLen(sockeid, NULL);//读取数据个数
			if (len > 0)
			{
				CH57xNET_SocketRecv(sockeid, net_tcp_client->client_recv_buf, &len);//提取数据
				if(net_tcp_client->received_back != NULL)
				{
					net_tcp_client->received_back(net_tcp_client->socke_id, net_tcp_client->socke_index, net_tcp_client->client_recv_buf, len);
				}
			}
    }
		if(initstat & SINT_STAT_CONNECT)/* TCP连接*/
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
		if(initstat & SINT_STAT_DISCONNECT)/* TCP断开 */
		{/* 产生此中断，CH579库内部会将此socket清除，置为关闭,需要重新创建*/
			net_tcp_client->connected_state=0;
			
//			net_tcp_client_creat_socket(net_tcp_client, NULL,NULL,NULL);//重新创建
			debug_printf("tcp disconnected:%d-%d\r\n",net_tcp_client->socke_id, net_tcp_client->socke_index);
			net_tcp_client->socke_id=255;
			if(net_tcp_client->disconnected_back != NULL)
			{
				net_tcp_client->disconnected_back(net_tcp_client->socke_id, net_tcp_client->socke_index);
			}
		}
		if(initstat & SINT_STAT_TIM_OUT)/* TCP超时中断 */
		{/* 产生此中断，CH579库内部会将此socket清除，置为关闭,需要重新创建*/
			net_tcp_client->connected_state=0;
			
//			net_tcp_client_creat_socket(net_tcp_client, NULL,NULL,NULL);//重新创建
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
 * Description    : 事件处理函数
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_HandleGlobalInt(void) {
  UINT8 initstat;
  UINT8 i;
  UINT8 socketinit;
  initstat = CH57xNET_GetGlobalInt(); /* 读全局中断状态并清除 */

  if(initstat & GINT_STAT_UNREACH)  /* 不可达中断 */
   {  
	   debug_printf("UnreachCode ：%ld\n",CH57xInf.UnreachCode); /* 查看不可达代码 */
     debug_printf("UnreachProto ：%ld\n",CH57xInf.UnreachProto); /* 查看不可达协议类型 */
     debug_printf("UnreachPort ：%ld\n",CH57xInf.UnreachPort); /* 查询不可达端口 */
   }
  if(initstat & GINT_STAT_IP_CONFLI) { /* IP冲突中断 */
   }
  if(initstat & GINT_STAT_PHY_CHANGE)  /* PHY改变中断 */
   { 
		 i = CH57xNET_GetPHYStatus();         /* 获取PHY状态 */
     debug_printf("GINT_STAT_PHY_CHANGE %02x\n", i);
   }
  if(initstat & GINT_STAT_SOCKET) /* Socket中断 */
   {  
		for(i = 0; i < CH57xNET_MAX_SOCKET_NUM; i++) 
		  {
        socketinit = CH57xNET_GetSocketInt(i); /* 读socket中断并清零 */
        if(socketinit) 
				 {                      // 有事件需要处理
//           debug_printf("Socket Interupt!\r\n");
           net_tcp_client_data(&net_tcp_client, i, socketinit); // 处理TCP数据
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
   net_tcp_client.ip_addr_remote[0] = ip_addr_from_udp[0];           /*设置服务器的IP地址*/
   net_tcp_client.ip_addr_remote[1] = ip_addr_from_udp[1];
   net_tcp_client.ip_addr_remote[2] = ip_addr_from_udp[2];
   net_tcp_client.ip_addr_remote[3] = ip_addr_from_udp[3];
           /*设置端口号*/
   net_tcp_client.port_remote = port_from_udp; // 要连接的服务器端口号
   net_tcp_client.port_local = 2002;           // 本地端口号 正式需要随机
   debug_printf("ip_addr_remote : %d.%d.%d.%d  port_remote : %d\r\n",net_tcp_client.ip_addr_remote[0], net_tcp_client.ip_addr_remote[1],net_tcp_client.ip_addr_remote[2], net_tcp_client.ip_addr_remote[3],net_tcp_client.port_remote);

   net_tcp_client.socke_index = 0; // 给每个客户端设置一个标识(可以不设置)
   net_tcp_client.socke_id = 255;  // 初始化该值为255(必须设置下),设置为255就可以		
   debug_printf("Destination address:%d.%d.%d.%d port:%d\r\n",net_tcp_client.ip_addr_remote[0],net_tcp_client.ip_addr_remote[1],net_tcp_client.ip_addr_remote[2],net_tcp_client.ip_addr_remote[3], net_tcp_client.port_remote);	
   //net_tcp_client_creat_socket(&net_tcp_client, tcp_connected_back,tcp_disconnected_back, tcp_recv_back);
   //net_tcp_client_connect(&net_tcp_client);    

	 dhcp_callback();
	
}






