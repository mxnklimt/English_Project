#include "iostream"
using namespace std;
//#include"my_function.h"
#include"string.h"
struct Student
	//��������
{
	string name;
	int age;
	int score;
}s3;
//˳�㶨��ṹ��s3
int structdefine()
{
	struct Student s1;
	//�����ṹ�����ʱ��struct����ʡ��
	s1.name = "mxnklimt";
	s1.age = 18;
	s1.score = 100;
	cout << "name ��" << s1.name << endl;
	cout << "age ��" << s1.age << endl;
	cout << "score ��" << s1.score << endl;

	struct Student s2 = { "xmnklimt",19,80 };
	cout << "name ��" << s2.name << endl;
	cout << "age ��" << s2.age << endl;
	cout << "score ��" << s2.score << endl;


	s3.name = "klimt";
	s3.age = 14;
	s3.score = 10;
	cout << "name ��" << s3.name << endl;
	cout << "age ��" << s3.age << endl;
	cout << "score ��" << s3.score << endl;
	system("pause");
	return 0;

}