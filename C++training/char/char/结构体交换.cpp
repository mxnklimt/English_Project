#include "iostream"
using namespace std;

struct Student
	//��������
{
	string name;
	int age;
	int score;
};

int main()
{
	struct Student arr[3] =
	{
		{"a",18,80},
		{"b",3,68},
		{"c",6,35},
	};
	Student temp = arr[2];
	arr[2] = arr[1];
	arr[1] = temp;

	for (int i = 0; i < 3; i++)
	{
		cout << "name ��" << arr[i].name << endl;
		cout << "age ��" << arr[i].age << endl;
		cout << "score ��" << arr[i].score << endl;
		cout << endl;
	}

	system("pause");
	return 0;

}