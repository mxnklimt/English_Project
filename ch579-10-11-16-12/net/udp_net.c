#define net_udp_c
/********************************** (C) COPYRIGHT ******************************
 * File Name          : UDP_Client.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/12/01
 * Description        : CH57xNET����ʾ�ļ�
 *                      (1)��CH579 Examples by KEIL;
 *                      (2)������1��������Ϣ,115200bps;
 *                      (3)��������������ʾUDP
 *�ͻ���ͨѶ����Ƭ���յ����ݺ󣬻ش���Ŀ��IP��λ��
 *                      (4)����\�رյ�����Ϣ��ӡ��IDE�궨��������
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ�����*/

#include "net_udp.h"
#if 1 // �Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif
#define RECE_BUF_LEN 100 /* ���ջ������Ĵ�С */

UINT8 SocketRecvBuf[RECE_BUF_LEN]; /* socket���ջ����� */
UINT8 MyBuf[RECE_BUF_LEN];         /* ����һ����ʱ������ */

UINT8 DESIP[4] = {255, 255, 255, 255}; /* Ŀ��IP��ַ */
char UDP_Received_Flag = 0;            // ȫ��ͨ�ñ���
char ip_addr_from_udp[4] = {0};        // ȫ��ͨ�ñ���
unsigned short port_from_udp;          // ȫ��ͨ�ñ���
UINT8 broadcast_ip[4] = {255, 255, 255, 255}; // ȫ��ͨ�ñ���
UINT16 broadcast_port = 10001;                // ȫ��ͨ�ñ���
UINT8 broadcast_payload[18] = {0xFF, 0xEE, 0x04, 0x00, 0x00, 0x00,0x00, 0x00, 0x00, 0x08, 0x00, 0xFF,0x03, 0x00, 0x02, 0x00, 0x01, 0x02};// ȫ��ͨ�ñ���
UINT32 broadcast_len = 18;                    // ȫ��ͨ�ñ���
UINT8 SocketId;                               // ȫ��ͨ�ñ���
/*******************************************************************************
 * Function Name  : mStopIfError
 * Description    : ����ʹ�ã���ʾ�������
 * Input          : iError �������
 * Output         : None
 * Return         : None
 *******************************************************************************/
void mStopIfError(UINT8 iError) 
{
  if (iError == CH57xNET_ERR_SUCCESS)
    return;                                        /* �����ɹ� */
  PRINT("mStopIfError: %02X\r\n", (UINT16)iError); /* ��ʾ���� */
}

/*******************************************************************************
 * Function Name  : CH57xNET_CreatUpdSocket
 * Description    : ����UDP socket
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_CreatUdpSocket(UINT8 *SocketId)
{
  UINT8 i;
  SOCK_INF TmpSocketInf; /* ������ʱsocket���� */
	debug_printf("TmpSocketInf\r\n");
  memset((void *)&TmpSocketInf, 0,sizeof(SOCK_INF)); /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
  memcpy((void *)TmpSocketInf.IPAddr, DESIP, 4); /* ����Ŀ��IP��ַ */
  TmpSocketInf.DesPort = 10001;                  /* ����Ŀ�Ķ˿� */
  TmpSocketInf.SourPort = 3000;                  /* ����Դ�˿� */
  TmpSocketInf.ProtoType = PROTO_TYPE_UDP;       /* ����socekt���� */
  TmpSocketInf.RecvStartPoint =(UINT32)SocketRecvBuf;              /* ���ý��ջ������Ľ��ջ����� */
  TmpSocketInf.RecvBufLen = RECE_BUF_LEN; /* ���ý��ջ������Ľ��ճ��� */
  i = CH57xNET_SocketCreat(SocketId,&TmpSocketInf); /* ����socket�������ص�socket����������SocketId�� */
	debug_printf("SocketCreat id = %d\r\n", *SocketId);
  mStopIfError(i);    /* ������ */
}

/*******************************************************************************
 * Function Name  : CH57xNET_HandleSockInt
 * Description    : Socket�жϴ�����
 * Input          : sockeid  socket����
 *                ��initstat �ж�״̬
 * Output         : None
 * Return         : None
 *******************************************************************************/
void CH57xNET_HandleSockInt(UINT8 sockeid, UINT8 initstat) {
  UINT32 len;
  if(sockeid == SocketId)
	{
  if (initstat & SINT_STAT_RECV) /* �����ж� */
  {
    len = CH57xNET_SocketRecvLen(sockeid, NULL); /* ��ѯ���� */

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
     if (initstat & SINT_STAT_RECV) /* �����ж� */
      {
        len = CH57xNET_SocketRecvLen(sockeid, NULL); /* ��ѯ���� */
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
