#include "iostream"
using namespace std;

int addx2_ex()
{
	int a = 10;
	int b = 3;
	cout << a % b << endl;
	//ȡ��

	//ǰ�õ��������ñ���+1��Ȼ����б��ʽ���㣬++a
	//���õ��������ʽ�����㣬���ñ���+1��a++
	int c = 10;
	c = ++a * 10;
	cout << "c = " << c << endl;
	a = 10;
	c = a++ * 10;
	cout << "c = " << c << endl;

	system("pause");

	return 0;
}