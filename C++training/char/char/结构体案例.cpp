#include "iostream"
using namespace std;

struct Student
	//��������
{
	string name;
	int score;
};
struct Teacher
{
	string name;
	struct Student sArray[5];
};

void AllocateSpace(struct Teacher teacher[],int len)
{
	string nameseed = "123456789";
	for (int i = 0; i < len; i++)
	{
		teacher[i].name = "Teacher_";
		teacher[i].name += nameseed[i];
		for (int j = 0; j < 5; j++)
		{
			teacher[i].sArray[j].name = "Student_";
			teacher[i].sArray[j].name += nameseed[j];
			teacher[i].sArray[j].score = rand()%100;
		}
	}
}
//Allocate����

void PrintInfo(struct Teacher teacher[], int len)
{
	for (int i = 0; i < len; i++)
	{
		cout << "��ʦ������  " << teacher[i].name << endl;

		for (int j = 0; j < 5; j++)
		{
			cout << "	ѧ��������  " << teacher[i].sArray[j].name <<
				"	���Է�����  " << teacher[i].sArray[j].score << endl;
		}
	}
}
int Struct_ex()
{
	struct Teacher teacher[3];
	int len = sizeof(teacher) / sizeof(teacher[0]);
	AllocateSpace(teacher, len);
	PrintInfo(teacher, len);
	system("pause");
	return 0;

}