#define net_udp_c
/********************************** (C) COPYRIGHT ******************************
 * File Name          : UDP_Client.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/01
 * Description        : CH57xNET库演示文件
 *                      (1)、CH579 Examples by KEIL;
 *                      (2)、串口1输出监控信息,115200bps;
 *                      (3)、本程序用于演示UDP
 *客户端通讯，单片机收到数据后，回传给目标IP上位机
 *                      (4)、打开\关闭调试信息打印在IDE宏定义中设置
 *******************************************************************************/

/******************************************************************************/
/* 头文件包含*/

#include "net_udp.h"
#if 1 // 是否打印日志
#define debug_printf printf
#else
#define debug_printf(...)
#endif
#define RECE_BUF_LEN 100 /* 接收缓冲区的大小 */

UINT8 SocketRecvBuf[RECE_BUF_LEN]; /* socket接收缓冲区 */
UINT8 MyBuf[RECE_BUF_LEN];         /* 定义一个临时缓冲区 */

UINT8 DESIP[4] = {255, 255, 255, 255}; /* 目的IP地址 */
char UDP_Received_Flag = 0;            // 全局通用变量
char ip_addr_from_udp[4] = {0};        // 全局通用变量
unsigned short port_from_udp;          // 全局通用变量
UINT8 broadcast_ip[4] = {255, 255, 255, 255}; // 全局通用变量
UINT16 broadcast_port = 10001;                // 全局通用变量
UINT8 broadcast_payload[18] = {0xFF, 0xEE, 0x04, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x08, 0x00, 0xFF,0x03, 0x00, 0x02, 0x00, 0x01, 0x02};// 全局通用变量
UINT32 broadcast_len = 18;                    // 全局通用变量
UINT8 SocketId;                               // 全局通用变量
/*******************************************************************************
 * Function Name  : mStopIfError
 * Description    : 调试使用，显示错误代码
 * Input          : iError 错误代码
 * Output         : None
 * Return         : None
 *******************************************************************************/
void mStopIfError(UINT8 iError) 
{
  if (iError == CH57xNET_ERR_SUCCESS)
    return;                                        /* 操作成功 */
  PRINT("mStopIfError: %02X\r\n", (UINT16)iError); /* 显示错误 */
}

/*******************************************************************************
 * Function Name  : CH57xNET_CreatUpdSocket
 * Description    : 创建UDP socket
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_CreatUdpSocket(UINT8 *SocketId)
{
  UINT8 i;
  SOCK_INF TmpSocketInf; /* 创建临时socket变量 */
	debug_printf("TmpSocketInf\r\n");
  memset((void *)&TmpSocketInf, 0,sizeof(SOCK_INF)); /* 库内部会将此变量复制，所以最好将临时变量先全部清零 */
  memcpy((void *)TmpSocketInf.IPAddr, DESIP, 4); /* 设置目的IP地址 */
  TmpSocketInf.DesPort = 10001;                  /* 设置目的端口 */
  TmpSocketInf.SourPort = 3000;                  /* 设置源端口 */
  TmpSocketInf.ProtoType = PROTO_TYPE_UDP;       /* 设置socekt类型 */
  TmpSocketInf.RecvStartPoint =(UINT32)SocketRecvBuf;              /* 设置接收缓冲区的接收缓冲区 */
  TmpSocketInf.RecvBufLen = RECE_BUF_LEN; /* 设置接收缓冲区的接收长度 */
  i = CH57xNET_SocketCreat(SocketId,&TmpSocketInf); /* 创建socket，将返回的socket索引保存在SocketId中 */
	debug_printf("SocketCreat id = %d\r\n", *SocketId);
  mStopIfError(i);    /* 检查错误 */
}

/*******************************************************************************
 * Function Name  : CH57xNET_HandleSockInt
 * Description    : Socket中断处理函数
 * Input          : sockeid  socket索引
 *                ：initstat 中断状态
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_HandleSockInt(UINT8 sockeid, UINT8 initstat) {
  UINT32 len;
  if(sockeid == SocketId)
	{
  if (initstat & SINT_STAT_RECV) /* 接收中断 */
  {
    len = CH57xNET_SocketRecvLen(sockeid, NULL); /* 查询长度 */

    CH57xNET_SocketRecv(sockeid, MyBuf, &len);
		if(MyBuf[11] == 0xFF && MyBuf[12] == 0x03)
		{
		debug_printf("UDP_Received_Flag = 1111 \r\n");
		ip_addr_from_udp[0] = MyBuf[17];
		ip_addr_from_udp[1] = MyBuf[18];
		ip_addr_from_udp[2] = MyBuf[19];
		ip_addr_from_udp[3] = MyBuf[20];
		port_from_udp |= MyBuf[21] << 8;
		port_from_udp |= MyBuf[22];
		UDP_Received_Flag = 1;			
		} 

  }

}
}


void net_udp_socket_data(unsigned char sockeid, unsigned char initstat)
{
	unsigned long len;
  if(sockeid == SocketId)
	 {
     if (initstat & SINT_STAT_RECV) /* 接收中断 */
      {
        len = CH57xNET_SocketRecvLen(sockeid, NULL); /* 查询长度 */
        CH57xNET_SocketRecv(sockeid, MyBuf, &len);
		    if(MyBuf[11] == 0xFF && MyBuf[12] == 0x03)
		     {
		       debug_printf("UDP_Received_Flag = 1111 \r\n");
		       ip_addr_from_udp[0] = MyBuf[17];
		       ip_addr_from_udp[1] = MyBuf[18];
		       ip_addr_from_udp[2] = MyBuf[19];
		       ip_addr_from_udp[3] = MyBuf[20];
		       port_from_udp |= MyBuf[21] << 8;
		       port_from_udp |= MyBuf[22];
		       UDP_Received_Flag = 1;			
		       debug_printf("UDP_Received_Flag = 1111 \r\n");					 
		     } 
      }
   }
}


/*********************************** endfile **********************************/
