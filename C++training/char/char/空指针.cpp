#include "iostream"
using namespace std;

int empty_p()
{
	//空指针用于给指针变量初始化
	//空指针是不可以进行访问的
	//0-255之间的内存编号是系统占用的，因此不可以访问
	int* p = NULL;



	system("pause");
	return 0;

}