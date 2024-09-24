#include "iostream"
using namespace std;
struct Student
	//ÀàĞÍÃû³Æ
{
	string name;
	int age;
	int score;
};
struct teacher
{
	int id;
	struct Student stu;
};
int StructNested()
{
	teacher mxn;
	mxn.id = 242040191;
	mxn.stu.name = "zhu";
	mxn.stu.age = 22;
	mxn.stu.score = 110;

	teacher *p = &mxn;
	cout << "name £º" << p->stu.name << endl;
	cout << "age £º" << p->stu.age << endl;
	cout << "score £º" << p->stu.score << endl;


	system("pause");
	return 0;

}