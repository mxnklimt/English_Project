#include "iostream"
using namespace std;
#include"my_function.h"
struct student
{
	string name;
	int age;
	int score;
};

int StructArray()
{
	struct student arr[3] =
	{
		{"a",18,80},
		{"b",3,68},
		{"c",6,35},
	};
	//结构体数组
	
	arr[2].name = "mxnklimt";
	arr[2].age = 100;
	arr[2].score = 100;

	for (int i = 0; i < 3; i++)
	{
		cout << "name ：" << arr[i].name << endl;
		cout << "age ：" << arr[i].age << endl;
		cout << "score ：" << arr[i].score << endl;
		cout << endl;
	}

	system("pause");
	return 0;

}