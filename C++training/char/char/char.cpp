#include "iostream"
using namespace std;

int charex()
{
	char ch = 'c';
	cout << ch << endl;

	//�ַ��ͱ�����ֻ����һ���ַ����������ַ���
	//�ַ��ͱ���ռ��һ���ֽ�
	//��ʾ�ַ��ͱ�����ʱ���õ����Ž��ַ�����������Ҫ��˫����
	cout << "c����ַ���Ӧ��ASCII����: " << (int)ch << endl;
	//a - 97
	//A - 65

	ch = 65;
	cout << ch << endl;
	//����ֱ����ASCII���ַ��ͱ�����ֵ
	system("pause");
	return 0;

}