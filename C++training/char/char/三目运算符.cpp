#include "iostream"
using namespace std;

int sanmu()

{
	int a = 10;
	int b = 20;
	int c = 0;
	c = (a > b ? a : b);
	//如果a>b,c=a;如果a!>b,c=b
	cout << "c = " << c << endl;

	(a > b ? a : b) = 100;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	system("pause");
	return 0;
}