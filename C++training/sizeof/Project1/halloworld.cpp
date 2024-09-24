#include "iostream"
//stream小溪
using namespace std;



int main()
{
	short num1 = 10;

	cout <<  "short占用的内存空间：" << sizeof(short) << endl;
	cout << "int占用的内存空间：" << sizeof(int) << endl;
	cout << "long占用的内存空间：" << sizeof(long) << endl;
	cout << "long long占用的内存空间：" << sizeof(long long) << endl;
	

	system("pause");//在编写的c++程序中，如果是窗口，有时会一闪就消失了，如果不想让其消失，在程序结尾处添加：system("pause");
	
	  
	return 0;

}