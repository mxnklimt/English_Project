#include "iostream"
using namespace std;

int p_ex()
{
	//ָ�����һ����ַ
	int* p;
	//��ָ���¼����a�ĵ�ַ
	int a = 10;
	p = &a;
	cout << "a�ĵ�ַΪ ��" << &a << endl;
	cout << "ָ��pΪ ��" << p << endl;
	//����p��ֵ����a�ĵ�ַ
	//ָ��ǰ��һ��*��������ã�����ָ��ָ����ڴ��е�����
	cout << "*pΪ ��" << *p << endl;
	system("pause");
	return 0;

}

