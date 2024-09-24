#define net_dhcp_c

#include "net_dhcp.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif

UINT8 net_dhcp_ip[4] = {0};   /* CH579IP��ַ */
UINT8 net_dhcp_gw[4];   /* CH579���� */
UINT8 net_dhcp_mask[4]; /* CH579�������� */
uint32_t main_len1=0;                          // ȫ��ͨ�ñ���

void (*net_dhcp_callback)();

char net_dhcp_phy_state=0;

/*******************************************************************************
* Function Name  : CH57xNET_DHCPCallBack
* Description    : DHCP�ص�����
* Input          : None
* Output         : None
* Return         : ִ��״̬
*******************************************************************************/
UINT8 CH57xNET_DHCPCallBack(UINT8 status,void *arg)
{
	UINT8 *p;
	if(!status){/* �ɹ�*/
		p = arg;
		debug_printf("DHCP Success\n");
		memcpy(net_dhcp_ip,p,4);//��ȡ�����IP��ַ
		memcpy(net_dhcp_gw,&p[4],4);//��ȡ����
		memcpy(net_dhcp_mask,&p[8],4);//��ȡ��������
		debug_printf("IPAddr = %d.%d.%d.%d \r\n",(UINT16)net_dhcp_ip[0],(UINT16)net_dhcp_ip[1],(UINT16)net_dhcp_ip[2],(UINT16)net_dhcp_ip[3]);
		debug_printf("GWIPAddr = %d.%d.%d.%d \r\n",(UINT16)net_dhcp_gw[0],(UINT16)net_dhcp_gw[1],(UINT16)net_dhcp_gw[2],(UINT16)net_dhcp_gw[3]);
		debug_printf("IPAddr = %d.%d.%d.%d \r\n",(UINT16)net_dhcp_mask[0],(UINT16)net_dhcp_mask[1],(UINT16)net_dhcp_mask[2],(UINT16)net_dhcp_mask[3]);
		debug_printf("DNS1: %d.%d.%d.%d\r\n",p[12],p[13],p[14],p[15]);
		debug_printf("DNS2: %d.%d.%d.%d\r\n",p[16],p[17],p[18],p[19]);
		if(net_dhcp_callback!=NULL)
		 {
			 net_dhcp_callback();
		 }
		else
		 {
						main_len1 = 1;
		 }
	}
	else
		{/* �������жϣ�CH57xNET���ڲ��Ὣ��socket�������Ϊ�ر�*/
		debug_printf("DHCP Fail %02x\n",status);
	}                                                      
	return 0;
}



/*ʹ��˵��
void dhcp_callback(void)
{
	ÿ��dhcp�ɹ������˺���,�������Ҫ��ʼ�������������ʼ��
}

��net_dhcp_while�����ŵ�while(1)ѭ������
while(1)
{
	net_dhcp_while(dhcp_callback);//DHCP
}
*/
void net_dhcp_while(void *callback)
{
	if(net_dhcp_phy_state != CH57xInf.PHYStat){//��������״̬�ı�
		net_dhcp_phy_state = CH57xInf.PHYStat;
		if(net_dhcp_phy_state==2)
			{//����
			debug_printf("net_phy_connected\r\n");
			net_dhcp_callback = callback;
			CH57xNET_DHCPStart(CH57xNET_DHCPCallBack);/* ����DHCP */ 
		}
		else
			{//�Ͽ�
			debug_printf("net_phy_disconnected\r\n");
			CH57xNET_DHCPStop();//�ر�DHCP
		}
	}
}

