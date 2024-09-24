#include "iostream"
using namespace std;
#include"my_function.h"
#include"string.h"
struct Student
	//类型名称
{
	string name;
	int age;
	int score;
};

int Structp()
{
	struct Student s1;
	//创建结构体变量时，struct可以省略
	s1.name = "mxnklimt";
	s1.age = 18;
	s1.score = 100;
	

	Student *p = &s1;
	//结构体指针*P，指向结构体变量s1

	cout << "name ：" << p->name << endl;
	cout << "age ：" << p->age << endl;
	cout << "score ：" << p->score << endl;
	//结构体指针访问
	//结构体指针可以通过->操作符访问成员
	system("pause");
	return 0;

}