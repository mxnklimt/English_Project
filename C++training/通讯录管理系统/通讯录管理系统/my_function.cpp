#include "iostream"
using namespace std;
#include"my_struct.h"
#include"my_function.h"

int FindName(AddressBook* abs, string name);


void AddPerson(AddressBook *abs)
{
	if (abs->number == capacity)
	{
		cout << "通讯录已满，添加失败" << endl;
		return;
	}
	else
	{
		//添加联系人
		string name;
		cout << "请输入姓名： " << endl;
		cin >> name;
		abs->personarray[abs->number].name = name;

		int level;
		cout << "请输入level： " << endl;
		cin >> level;
		abs->personarray[abs->number].level = level;

		abs->number++;
		cout << "success" << endl;
		system("pause");
		//system("cls");

	}
}

void ShowAddressBook(AddressBook* abs)
{
	if (abs->number == 0)
	{
		cout << "联系人数据为空" << endl;
	}
	else
	{
		for (int i = 0; i< abs->number;i++)
		{
			cout << abs->number <<". 姓名： " << abs->personarray[i].name << " Level: "<<abs->personarray[i].level << endl;
			

		}
		system("pause");
	}
}
void DeleteData(AddressBook *abs)
{
	cout << "请输入你要删除的联系人名字" << endl;
	string name;
	cin >> name;
	int deletenum=FindName(abs, name);
	if (deletenum == -1)
	{
		cout << "不存在" << endl;
	}
	else
	{

		for(int i=deletenum;i<abs->number+1;i++)
		{ 

			abs->personarray[i] = abs->personarray[i + 1];
		}
		abs->number--;
	}

}
void FindPeople(AddressBook* abs)
{
	cout << "请输入你要查找的联系人名字" << endl;
	string name;
	cin >> name;
	int Findnum = FindName(abs, name);
	if (Findnum == -1)
	{
		cout << "不存在" << endl;
		system("pause");
	}
	else
	{
		cout << Findnum << ". 姓名： " << abs->personarray[Findnum].name << " Level: " << abs->personarray[Findnum].level << endl;
		system("pause");
	}
}
void Revise(AddressBook* abs)
{
	cout << "请输入你要修改的联系人名字" << endl;
	string name;
	cin >> name;
	int Findnum = FindName(abs, name);
	if (Findnum == -1)
	{
		cout << "不存在" << endl;
		system("pause");
	}
	else
	{
		string name;
		cout << "请输入姓名： " << endl;
		cin >> name;
		abs->personarray[Findnum].name = name;

		int level;
		cout << "请输入level： " << endl;
		cin >> level;
		abs->personarray[Findnum].level = level;

		cout << Findnum << ". 姓名： " << abs->personarray[Findnum].name << " Level: " << abs->personarray[Findnum].level << endl;
		system("pause");
	}
	
}
void ResetData(AddressBook* abs)
{
	abs->number = 0;
	cout << "已经清空" << endl;
	system("pause");
	system("cls");
}
int FindName(AddressBook* abs,string name)
{

	for (int i = 0; i < abs->number; i++) 
	{
		if (abs->personarray[i].name == name)
		{
			return i;
		}
	}
	return -1;
}

void show()
{
	cout << "**************************" << endl;
	cout << "*****  1.添加联系人  *****" << endl;
	cout << "*****  2.显示联系人  *****" << endl;
	cout << "*****  3.删除联系人  *****" << endl;
	cout << "*****  4.查找联系人  *****" << endl;
	cout << "*****  5.修改联系人  *****" << endl;
	cout << "*****  6.清空联系人  *****" << endl;
	cout << "*****  0.退出通讯录  *****" << endl;
	cout << "**************************" << endl;
}