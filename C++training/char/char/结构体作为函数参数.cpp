#include "iostream"
using namespace std;

struct Student
	//��������
{
	string name;
	int age;
	int score;
}s1;

void PrintStudent(Student stu)
{
	cout << "name ��" << stu.name << endl;
	cout << "age ��" << stu.age << endl;
	cout << "score ��" << stu.score << endl;
}
//��������ֵ���ݺ�ռ���ڴ��Խ��

void PrintStudentAddress(Student *p)
{
	cout << "name ��" << p->name << endl;
	cout << "age ��" << p->age << endl;
	cout << "score ��" << p->score << endl;
}
//�����βθ�Ϊָ�룬���Խ�ʡ�ڴ�ռ䣬���Ҳ��Ḵ���µĸ�������

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