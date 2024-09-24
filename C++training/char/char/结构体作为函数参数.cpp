#include "iostream"
using namespace std;

struct Student
	//类型名称
{
	string name;
	int age;
	int score;
}s1;

void PrintStudent(Student stu)
{
	cout << "name ：" << stu.name << endl;
	cout << "age ：" << stu.age << endl;
	cout << "score ：" << stu.score << endl;
}
//数据量大，值传递后占用内存就越多

void PrintStudentAddress(Student *p)
{
	cout << "name ：" << p->name << endl;
	cout << "age ：" << p->age << endl;
	cout << "score ：" << p->score << endl;
}
//函数形参改为指针，可以节省内存空间，而且不会复制新的副本出来

int PrintStudentShow()
{
	s1.name = "mxn";
	s1.age = 23;
	s1.score = 100;

	PrintStudent(s1);

	Student* p = &s1;
	PrintStudentAddress(p);


	system("pause");
	return 0;

}