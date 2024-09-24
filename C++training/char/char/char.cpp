#include "iostream"
using namespace std;

int charex()
{
	char ch = 'c';
	cout << ch << endl;

	//字符型变量内只能有一个字符，不能是字符串
	//字符型变量占用一个字节
	//显示字符型变量的时候用单引号将字符括起来，不要用双引号
	cout << "c这个字符对应的ASCII编码: " << (int)ch << endl;
	//a - 97
	//A - 65

	ch = 65;
	cout << ch << endl;
	//可以直接用ASCII给字符型变量赋值
	system("pause");
	return 0;

}