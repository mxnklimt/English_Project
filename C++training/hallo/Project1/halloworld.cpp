#include "iostream"
//stream小溪
using namespace std;

/*
常量
1. #define
2. const
*/
#define Day 7
//宏定义不加；

int main()
{
	std::cout << "hallo world" << endl;

	cout << Day << "days a week" << endl;

	int a = 10;
	cout << "a is " << a << endl; //endl== end line

	const int month = 12;
	cout << month << " month a year" << endl;

	system("pause");//在编写的c++程序中，如果是窗口，有时会一闪就消失了，如果不想让其消失，在程序结尾处添加：system("pause");
	
	  
	return 0;

}