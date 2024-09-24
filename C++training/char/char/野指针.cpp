#include "iostream"
using namespace std;

int danger_p()
{
	int* p = (int*)0x1100;
	//野指针，不是我们申请的空间
	//不能访问野指针


	system("pause");
	return 0;

}