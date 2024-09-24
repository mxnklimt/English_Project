#include "iostream"
using namespace std;

int p_ex()
{
	//指针就是一个地址
	int* p;
	//让指针记录变量a的地址
	int a = 10;
	p = &a;
	cout << "a的地址为 ：" << &a << endl;
	cout << "指针p为 ：" << p << endl;
	//所以p的值就是a的地址
	//指针前加一个*代表解引用，代表指针指向的内存中的数据
	cout << "*p为 ：" << *p << endl;
	system("pause");
	return 0;

}

