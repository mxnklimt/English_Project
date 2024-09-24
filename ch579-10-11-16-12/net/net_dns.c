#define net_dns_c

#include "net_dns.h"

#if 1 //�Ƿ��ӡ��־
#define debug_printf printf
#else
#define debug_printf(...)
#endif


unsigned char  net_dns_socket_index=255;             /* ����socket����*/

unsigned char  dns_server_ip[4]={114, 114, 114, 114};   /*DNS�������������ʵ��DNS�������޸�*/
unsigned short dns_server_port   = 53;         /*DNSĬ�϶˿�*/
unsigned short dns_local_port  = 1000;         /*Socket ���ض˿� */

unsigned short nds_msg_id = 0x1100;               /*��ʶ*/

/*����DNS��Э������*/
#define    net_dns_buf_len    512
unsigned char  net_dns_buf[net_dns_buf_len];
/*����UDP�������ݰ�*/
#define net_dns_socket_buf_len 536
unsigned char net_dns_socket_buf[net_dns_socket_buf_len];
/*����udp���յ�����*/
#define net_dns_socket_recv_buf_len 512
unsigned char net_dns_socket_recv_buf[net_dns_socket_recv_buf_len];

unsigned char * net_dns_ip_copy;//��¼���IP�������ַ
unsigned char net_dns_start=0;//����Ƿ��Ѿ�����
int net_dns_time_out_cnt=0;//��ʱ����

//UINT8 domain_ip

/*********************************************************************************
* Function Name  : get16
* Description    : ��������UINT8����תΪUINT16��ʽ����
* Input          : UINT8 ��������
* Output         : None
* Return         : ת�����UINT16��������
**********************************************************************************/
unsigned short net_dns_get16(unsigned char * s)
{
  unsigned short i;
  i = *s++ << 8;
  i = i + *s;
  return i;
}

/**********************************************************************************
* Function Name  : ParseName
* Description    : ��������������
* Input          : msg            -ָ���ĵ�ָ��
                   compressed    -ָ��������������ָ��
                   buf            -������ָ�룬���ڴ��ת��������
* Output         : None
* Return         : ѹ�����ĵĳ���
**********************************************************************************/
int net_dns_parse_name(unsigned char * msg, unsigned char * compressed, char * buf)
{
  unsigned short slen;                                                                            /* ��ǰƬ�γ���*/
  unsigned char * cp;
  int clen = 0;                                                                           /* ѹ���������� */
  int indirect = 0;
  int nseg = 0;                                                                           /* �������ָ��Ƭ������ */
  cp = compressed;
  for (;;)
  {
    slen = *cp++;                                                                          /* ���ֽڵļ���ֵ*/
    if (!indirect) clen++;
    if ((slen & 0xc0) == 0xc0)                                                             /*�����ֽڸ�������Ϊ1������ѹ����ʽ*/
    {
      if (!indirect)
        clen++;
        indirect = 1;
        cp = &msg[((slen & 0x3f)<<8) + *cp];                                              /*�������ֽ���ֵָ��ƫ�Ƶ�ָ��λ��*/
        slen = *cp++;
    } 
    if (slen == 0)                                                                        /*����Ϊ0������*/
        break;
    if (!indirect) clen += slen;
    while (slen-- != 0) *buf++ = (char)*cp++;
    *buf++ = '.';
    nseg++;
  }
   if (nseg == 0)
   {
     *buf++ = '.';
   }
   *buf++ = '\0';
   return clen;                                                                            /*ѹ�����ĳ��� */
}

/*********************************************************************************
* Function Name  : DnsQuestion
* Description    : ������Ӧ�����е������¼����
* Input          : msg          -ָ����Ӧ���ĵ�ָ��
                   cp           -ָ�������¼��ָ��
* Output         : None
* Return         :ָ����һ��¼��ָ��
**********************************************************************************/
unsigned char * net_dns_question(unsigned char * msg, unsigned char * cp)
{
  int len;
  char name[net_dns_buf_len];
  len = net_dns_parse_name(msg, cp, name);
  cp += len;
  cp += 2;                                                                                  /* ���� */
  cp += 2;                                                                                  /* �� */
  return cp;
}

/*********************************************************************************
* Function Name  : DnsAnswer
* Description    : ������Ӧ�����еĻش��¼����
* Input          : msg            -ָ����Ӧ���ĵ�ָ��
                   cp             -ָ��ش��¼��ָ��
                   psip           
* Output         : None
* Return         :ָ����һ��¼��ָ��
**********************************************************************************/
unsigned char * net_dns_answer(unsigned char * msg, unsigned char * cp, unsigned char * pSip)
{
  int len, type;
  char name[net_dns_buf_len];
  len = net_dns_parse_name(msg, cp, name);
  cp += len;
  type = net_dns_get16(cp);
  cp += 2;                                                                                   /* ���� */
  cp += 2;                                                                                   /* �� */
  cp += 4;                                                                                   /* ����ʱ�� */
  cp += 2;                                                                                   /* ��Դ���ݳ��� */
  switch (type)
  {
  case TYPE_A:
    pSip[0] = *cp++;
    pSip[1] = *cp++;
    pSip[2] = *cp++;
    pSip[3] = *cp++;
    break;
  case TYPE_CNAME:
  case TYPE_MB:
  case TYPE_MG:
  case TYPE_MR:
  case TYPE_NS:
  case TYPE_PTR:
    len = net_dns_parse_name(msg, cp, name);
    cp += len;
    break;
  case TYPE_HINFO:

  case TYPE_MX:

  case TYPE_SOA:

  case TYPE_TXT:
    break;
  default:
    break;
  }
  return cp;
}
/*********************************************************************************
* Function Name  : parseMSG
* Description    : ������Ӧ�����е���Դ��¼����
* Input          : msg            -ָ��DNS����ͷ����ָ��
                   cp             -ָ����Ӧ���ĵ�ָ��
                   pSip
* Output         : None
* Return         :�ɹ�����0�����򷵻�1
**********************************************************************************/
unsigned char net_dns_parse_msg(struct dhdr * pdhdr, unsigned char * pbuf, unsigned char * pSip)
{
  unsigned short tmp;
  unsigned short i;
  unsigned char * msg;
  unsigned char * cp;
  msg = pbuf;
  memset(pdhdr, 0, sizeof(pdhdr));
  pdhdr->id = net_dns_get16(&msg[0]);
  tmp = net_dns_get16(&msg[2]);
  if (tmp & 0x8000) pdhdr->qr = 1;
  pdhdr->opcode = (tmp >> 11) & 0xf;
  if (tmp & 0x0400) pdhdr->aa = 1;
  if (tmp & 0x0200) pdhdr->tc = 1;
  if (tmp & 0x0100) pdhdr->rd = 1;
  if (tmp & 0x0080) pdhdr->ra = 1;
  pdhdr->rcode = tmp & 0xf;
  pdhdr->qdcount = net_dns_get16(&msg[4]);
  pdhdr->ancount = net_dns_get16(&msg[6]);
  pdhdr->nscount = net_dns_get16(&msg[8]);
  pdhdr->arcount = net_dns_get16(&msg[10]);
  /* �����ɱ����ݳ��Ȳ���*/
  cp = &msg[12];
  for (i = 0; i < pdhdr->qdcount; i++)                                                      /* ��ѯ���� */
  {
    cp = net_dns_question(msg, cp);
  }  
  for (i = 0; i < pdhdr->ancount; i++)                                                      /* �ش� */
  {
    cp = net_dns_answer(msg, cp, pSip);
  }
  for (i = 0; i < pdhdr->nscount; i++)                                                      /*��Ȩ */
  {
    /*������*/  ;
  }  
  for (i = 0; i < pdhdr->arcount; i++)                                                      /* ������Ϣ */
  {
    /*������*/  ;
  }
  if(pdhdr->rcode == 0) return 0;                                                           /*rcode = 0:�ɹ�*/
  else return 1;
}

/*********************************************************************************
* Function Name  : put16
* Description    :UINT16 ��ʽ���ݰ�UINT8��ʽ�浽������
* Input          : s -�������׵�ַ
                   i -UINT16����
* Output         : None
* Return         : ƫ��ָ��
**********************************************************************************/
unsigned char * net_dns_put16(unsigned char * s, unsigned short i)
{
  *s++ = i >> 8;
  *s++ = i;
  return s;
}

/**********************************************************************************
* Function Name  : MakeDnsQuery
* Description    : ����DNS��ѯ����
  input          : op   - �ݹ�
*                  name - ָ���������ָ��
*                  buf  - DNS������.
*                  len  - ��������󳤶�.
* Output         : None
* Return         : ָ��DNS����ָ��
**********************************************************************************/
unsigned short net_dns_make_query_msg(unsigned short op, char * name, unsigned char * buf, unsigned short len)
{
  unsigned char *cp;
  char *cp1;
  char tmpname[net_dns_buf_len];
  char *dname;
  unsigned short p;
  unsigned short dlen;
  cp = buf;
  nds_msg_id++;
  cp = net_dns_put16(cp, nds_msg_id);                                                             /*��ʶ*/
  p = (op << 11) | 0x0100;      
  cp = net_dns_put16(cp, p);                                                                      /* 0x0100��Recursion desired */
  cp = net_dns_put16(cp, 1);                                                                      /*��������1*/
  cp = net_dns_put16(cp, 0);                                                                      /*��Դ��¼����0*/
  cp = net_dns_put16(cp, 0);                                                                      /*��Դ��¼����0*/
  cp = net_dns_put16(cp, 0);                                                                      /*������Դ��¼����0*/
  strcpy(tmpname, name);
  dname = tmpname;
  dlen = strlen(dname);
  for (;;)
  {                                                                                        /*����DNS������������ʽ����URIд�뵽buf����ȥ*/
    cp1 = strchr(dname, '.');
    if (cp1 != NULL) len = cp1 - dname;  
    else len = dlen;    
    *cp++ = len;      
    if (len == 0) break;    
    strncpy((char *)cp, dname, len);
    cp += len;
    if (cp1 == NULL)
    {
      *cp++ = 0;    
      break;
    }
    dname += len+1;                                                                        /*dname�׵�ַ����*/
    dlen -= len+1;                                                                         /*dname���ȼ�С*/
  }
  cp = net_dns_put16(cp, 0x0001);                                                                  /* type ��1------ip��ַ*/
  cp = net_dns_put16(cp, 0x0001);                                                                  /* class ��1-------��������ַ*/
  return ((UINT16)(cp - buf));
}


/**
* @brief   ����DNS���������ص�����
* @param   data  ���ص������׵�ַ
* @param   None
* @param   None
* @param   None
* @retval  0:�ɹ�; others:����
* @warning None
* @example 
**/
char net_dns_parse(unsigned char *data)
{
	struct dhdr dhp;
	return net_dns_parse_msg(&dhp, data, net_dns_ip_copy);
}


/**
* @brief   ��ʼ��socket
* @param   ipaddr  Ŀ�ĵ�ַ
* @param   desprot Ŀ�Ķ˿ں�
* @param   surprot ���ض˿ں�
* @retval  0:��ʼ���ɹ�; others:��ʼ��ʧ��
* @warning None
* @example 
**/
char net_dns_socket_init(unsigned char *addr ,unsigned short DesPort ,unsigned short SourPort)
{
	unsigned char state;                                                             
	SOCK_INF TmpSocketInf;                                                       /* ������ʱsocket���� */
	memset((void *)&TmpSocketInf,0,sizeof(SOCK_INF));                            /* ���ڲ��Ὣ�˱������ƣ�������ý���ʱ������ȫ������ */
	memcpy((void *)TmpSocketInf.IPAddr,addr,4);                                  /* ����Ŀ��IP��ַ */
	
	TmpSocketInf.DesPort = DesPort;                                              /* ����Ŀ�Ķ˿� */
	TmpSocketInf.SourPort = SourPort;                                            /* ����Դ�˿� */
	TmpSocketInf.ProtoType = PROTO_TYPE_UDP;                                     /* ����socekt���� */
	TmpSocketInf.RecvStartPoint = (unsigned long)net_dns_socket_buf;                         /* ���ý��ջ������Ľ��ջ����� */
	TmpSocketInf.RecvBufLen = net_dns_socket_buf_len ;                                     /* ���ý��ջ������Ľ��ճ��� */
	state = CH57xNET_SocketCreat((unsigned char *)(&net_dns_socket_index),&TmpSocketInf); /* ����socket�������ص�socket����������SockeId�� */
	if(state!=CH57xNET_ERR_SUCCESS)
	{
		debug_printf("net_dns_socket_init_error: %02X\r\n", (UINT16)state);
	}
	return state;
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
char net_dns_socket_send(unsigned char socketid, unsigned char *buf, unsigned long len)
{
	unsigned long length = len;
	return CH57xNET_SocketSend(socketid,buf,&length);
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
char net_dns_socket_close(unsigned char socketid)
{
	if(socketid!=255){//������socket
		net_dns_socket_index=255;
		return CH57xNET_SocketClose( socketid,TCP_CLOSE_NORMAL );
	}
	net_dns_socket_index=255;
	return 255;
}


/**
* @brief   DNS��ʱ���,�ŵ���ʱ�����������ѵ
* @param   None
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning �Ƽ��ŵ�1ms��ʱ��, ��ʱʱ��200ms�Ϳ���
* @example 
**/
void net_dns_loop(void)
{
	if(net_dns_start==1)
	{
		net_dns_time_out_cnt++;
		if(net_dns_time_out_cnt>200)//��ʱ200ms
		{
			net_dns_time_out_cnt=0;
			net_dns_start=0;
			
			net_dns_socket_close(net_dns_socket_index);
		}
	}
	else
	{
		net_dns_time_out_cnt=0;
	}
}


/**
* @brief   ���Ͳ�ѯDNSָ��
* @param   name  ����
* @param   ip    ������IP��ַ�洢�ĵ�ַ
* @param   None
* @param   None
* @retval  0:�ɹ�; others:����
* @warning None
* @example
**/
char net_dns_query(char *domain_name, unsigned char *ip)
{
	unsigned long len=0;
	if(net_dns_start==0 && CH57xInf.PHYStat>=2){//û����ִ��
		debug_printf("net_dns_query\r\n");
		net_dns_ip_copy = ip;
		if(net_dns_socket_init(dns_server_ip,dns_server_port,dns_local_port) == 0)
		{
			len = net_dns_make_query_msg(0,domain_name, net_dns_buf, net_dns_buf_len);
			if(net_dns_socket_send(net_dns_socket_index, net_dns_buf, len)!=0)
			{
				net_dns_socket_close(net_dns_socket_index);
				net_dns_start=0;
				return 2;
			}
			net_dns_start=1;
		}
		else
		{
			net_dns_socket_close(net_dns_socket_index);
			net_dns_start=0;
			return 1;
		}
	}
	else
	{
		return 3;
	}
	return 0;
}



/**
* @brief   DNS���ݽ�������
* @param   sockeid
* @param   None
* @param   None
* @param   None
* @retval  None
* @warning None
* @example 
**/
void net_dns_socket_data(unsigned char sockeid, unsigned char initstat)
{
	unsigned long len;
	char   err=0;
	
	if(sockeid == net_dns_socket_index)
	{
		if(initstat & SINT_STAT_RECV)//���յ�����
    {
			len = CH57xNET_SocketRecvLen(sockeid,NULL);//��ȡ���ݸ���
			if (len > 0)
			{
				CH57xNET_SocketRecv(sockeid,net_dns_socket_recv_buf,&len);//��ȡ����
				
				if(net_dns_start==1)
				{
					if((err=net_dns_parse(net_dns_socket_recv_buf)) == 0){//����DNS���ݰ�,�����ɹ����IP���ݻ��浽�û����õ���������
						
						net_dns_socket_close(net_dns_socket_index);
						net_dns_start=0;
						debug_printf("net_dns_ip=%d.%d.%d.%d\r\n",net_dns_ip_copy[0],net_dns_ip_copy[1],net_dns_ip_copy[2],net_dns_ip_copy[3]);
					}
					else{
						debug_printf("net_dns_parse_err=%d\r\n",err);
					}
				}
			}
    }
	}
}


/**
*@brief		���ڽ����ַ���IP,����1˵��ת���ɹ�,����0˵��IP���ɽ���,����������
*@param		str Ҫת��������
*@param		ip  ת����洢��λ��
*@return 	0:ת��ʧ��,����������   1:ת���ɹ�
*@example  net_dns_str_ip("192.168.0.1", &ip) ip[0]=192;ip[1]=168;ip[2]=0;ip[3]=1;
*/ 
uint8_t net_dns_str_ip(const char* str, void *ip)
{
	/* The count of the number of bytes processed. */
	int i;
	/* A pointer to the next digit to process. */
	const char * start;

	start = str;
	for (i = 0; i < 4; i++) {
			/* The digit being processed. */
			char c;
			/* The value of this byte. */
			int n = 0;
			while (1) {
					c = * start;
					start++;
					if (c >= '0' && c <= '9') {
							n *= 10;
							n += c - '0';
					}
					/* We insist on stopping at "." if we are still parsing
						 the first, second, or third numbers. If we have reached
						 the end of the numbers, we will allow any character. */
					else if ((i < 3 && c == '.') || i == 3) {
							break;
					}
					else {
							return 0;
					}
			}
			if (n >= 256) {
					return 0;
			}
			((uint8_t*)ip)[i] = n;
	}
	return 1;
}

