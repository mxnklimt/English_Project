#include "iostream"
using namespace std;

struct Student
	//类型名称
{
	string name;
	int age;
	int score;
};
//这里结尾如果定义s1是全局变量
//struct Student { ... } s1; 定义了一个结构体 Student 以及一个名为 s1 的全局变量。这个全局变量可能导致多个文件之间的冲突，特别是在多个源文件中包含了相同的声明时，就会出现链接器错误 LNK2005（重复定义）。
void PrintStudentAddress(const Student* p)
{
	//const使得类似stu->age=150的语句会报错，防止误修改
	cout << "name ：" << p->name << endl;
	cout << "age ：" << p->age << endl;
	cout << "score ：" << p->score << endl;
}
//函数形参改为指针，可以节省内存空间，而且不会复制新的副本出来

int Structconst()
{
	struct Student s1;
	//这样定义的是局部变量
	s1.name = "mxn";
	s1.age = 23;
	s1.score = 100;

	PrintStudentAddress(&s1);

	system("pause");
	return 0;

}