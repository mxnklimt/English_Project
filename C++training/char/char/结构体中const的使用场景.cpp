#include "iostream"
using namespace std;

struct Student
	//��������
{
	string name;
	int age;
	int score;
};
//�����β�������s1��ȫ�ֱ���
//struct Student { ... } s1; ������һ���ṹ�� Student �Լ�һ����Ϊ s1 ��ȫ�ֱ��������ȫ�ֱ������ܵ��¶���ļ�֮��ĳ�ͻ���ر����ڶ��Դ�ļ��а�������ͬ������ʱ���ͻ�������������� LNK2005���ظ����壩��
void PrintStudentAddress(const Student* p)
{
	//constʹ������stu->age=150�����ᱨ����ֹ���޸�
	cout << "name ��" << p->name << endl;
	cout << "age ��" << p->age << endl;
	cout << "score ��" << p->score << endl;
}
//�����βθ�Ϊָ�룬���Խ�ʡ�ڴ�ռ䣬���Ҳ��Ḵ���µĸ�������

int Structconst()
{
	struct Student s1;
	//����������Ǿֲ�����
	s1.name = "mxn";
	s1.age = 23;
	s1.score = 100;

	PrintStudentAddress(&s1);

	system("pause");
	return 0;

}