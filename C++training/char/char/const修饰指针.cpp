#include "iostream"
using namespace std;

int constp_ex()
{
	int a = 10;
	int b = 10;
	const int* p = &a;
	//常量指针，指针的指向可以修改，指针指向的值不能改
	
	//所以*p=20；是错误的，指针的值不可以改
	//p=&b是正确的，因为a值=b
	
	int* const q = &a;
	//指针常量，指针指向的值可以改，指针的指向不能修改
	
	//所以*p=20是正确的
	//p=&b是错误的

	const int* const m=&a;
	//const 既修饰指针又修饰常量，指针的指向和指向的值都不可以改
	system("pause");
	return 0;

} 