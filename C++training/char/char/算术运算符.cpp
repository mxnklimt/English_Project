#include "iostream"
using namespace std;

int addx2_ex()
{
	int a = 10;
	int b = 3;
	cout << a % b << endl;
	//取余

	//前置递增，先让变量+1，然后进行表达式运算，++a
	//后置递增，表达式先运算，再让变量+1，a++
	int c = 10;
	c = ++a * 10;
	cout << "c = " << c << endl;
	a = 10;
	c = a++ * 10;
	cout << "c = " << c << endl;

	system("pause");

	return 0;
}