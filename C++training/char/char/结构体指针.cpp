#include "iostream"
using namespace std;
#include"my_function.h"
#include"string.h"
struct Student
	//��������
{
	string name;
	int age;
	int score;
};

int Structp()
{
	struct Student s1;
	//�����ṹ�����ʱ��struct����ʡ��
	s1.name = "mxnklimt";
	s1.age = 18;
	s1.score = 100;
	

	Student *p = &s1;
	//�ṹ��ָ��*P��ָ��ṹ�����s1

	cout << "name ��" << p->name << endl;
	cout << "age ��" << p->age << endl;
	cout << "score ��" << p->score << endl;
	//�ṹ��ָ�����
	//�ṹ��ָ�����ͨ��->���������ʳ�Ա
	system("pause");
	return 0;

}