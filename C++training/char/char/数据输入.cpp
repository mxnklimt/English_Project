#include "iostream"
using namespace std;

int cin_ex()
{
	cout << "请给整形变量赋值" << endl;
	int a = 0;

	cin >> a;
	//赋值语句
	cout << "整型变量a = " << a<<endl;
	
	//ctrl+k+c 选中内容变为注释

	string str2 = "xmnklimt";
	cout << str2 << endl;
	cin >> str2;
	cout << "str2= " << str2 << endl;

	bool flag = false;
	cin >> flag;
	cout << "bool flag = " << flag << endl;

	 

	system("pause");
	return 0;

}