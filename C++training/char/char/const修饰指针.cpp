#include "iostream"
using namespace std;

int constp_ex()
{
	int a = 10;
	int b = 10;
	const int* p = &a;
	//����ָ�룬ָ���ָ������޸ģ�ָ��ָ���ֵ���ܸ�
	
	//����*p=20���Ǵ���ģ�ָ���ֵ�����Ը�
	//p=&b����ȷ�ģ���Ϊaֵ=b
	
	int* const q = &a;
	//ָ�볣����ָ��ָ���ֵ���Ըģ�ָ���ָ�����޸�
	
	//����*p=20����ȷ��
	//p=&b�Ǵ����

	const int* const m=&a;
	//const ������ָ�������γ�����ָ���ָ���ָ���ֵ�������Ը�
	system("pause");
	return 0;

} 