#include "iostream"
using namespace std;

int arrandp_ex()
{
	int arr[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int* p = arr;
	//arr����������׵�ַ
	cout << "����ָ�����������׵�ַ�� " << *p << endl;
	p++;
	cout << "����ָ���������ڶ���Ԫ�أ� " << *p << endl;

	
	//����ָ���������
	p--;
	for (int i = 0; i < 10; i++)
	{
		cout << *p << endl;
		p++;
	}

	system("pause");
	return 0;

}