#include "iostream"
using namespace std;

int wjt()
{
	int a = 10;
	cout << !a << endl;//0
	//c++�г���0��Ϊ��
	cout << !!a << endl;//1
	int b = 10;
	if ((a == 10) && (b == 10))
	{
		cout << "damn" << endl;

	}
	if ((a == 10) || (b == 9))
	{
		cout << "haha" << endl;

	}
	
	system("pause");
	return 0;

}