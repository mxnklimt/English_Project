#include "iostream"
using namespace std;

int s_ex()
{
	int a = 0;
	a += 2;
	//a+=2 ¾ÍÊÇ a=a+2
	cout << a<< endl;
	a = 0;
	a-= 2;
	cout << a << endl;
	a = 1;
	a *= 2;
	cout << a << endl;
	a /= 2;
	cout << a << endl;
	a %= 2;
	cout << a << endl;

	system("pause");
	return 0;
}