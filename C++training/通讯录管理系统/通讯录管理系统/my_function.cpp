#include "iostream"
using namespace std;
#include"my_struct.h"
#include"my_function.h"

int FindName(AddressBook* abs, string name);


void AddPerson(AddressBook *abs)
{
	if (abs->number == capacity)
	{
		cout << "ͨѶ¼���������ʧ��" << endl;
		return;
	}
	else
	{
		//�����ϵ��
		string name;
		cout << "������������ " << endl;
		cin >> name;
		abs->personarray[abs->number].name = name;

		int level;
		cout << "������level�� " << endl;
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
		cout << "��ϵ������Ϊ��" << endl;
	}
	else
	{
		for (int i = 0; i< abs->number;i++)
		{
			cout << abs->number <<". ������ " << abs->personarray[i].name << " Level: "<<abs->personarray[i].level << endl;
			

		}
		system("pause");
	}
}
void DeleteData(AddressBook *abs)
{
	cout << "��������Ҫɾ������ϵ������" << endl;
	string name;
	cin >> name;
	int deletenum=FindName(abs, name);
	if (deletenum == -1)
	{
		cout << "������" << endl;
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
	cout << "��������Ҫ���ҵ���ϵ������" << endl;
	string name;
	cin >> name;
	int Findnum = FindName(abs, name);
	if (Findnum == -1)
	{
		cout << "������" << endl;
		system("pause");
	}
	else
	{
		cout << Findnum << ". ������ " << abs->personarray[Findnum].name << " Level: " << abs->personarray[Findnum].level << endl;
		system("pause");
	}
}
void Revise(AddressBook* abs)
{
	cout << "��������Ҫ�޸ĵ���ϵ������" << endl;
	string name;
	cin >> name;
	int Findnum = FindName(abs, name);
	if (Findnum == -1)
	{
		cout << "������" << endl;
		system("pause");
	}
	else
	{
		string name;
		cout << "������������ " << endl;
		cin >> name;
		abs->personarray[Findnum].name = name;

		int level;
		cout << "������level�� " << endl;
		cin >> level;
		abs->personarray[Findnum].level = level;

		cout << Findnum << ". ������ " << abs->personarray[Findnum].name << " Level: " << abs->personarray[Findnum].level << endl;
		system("pause");
	}
	
}
void ResetData(AddressBook* abs)
{
	abs->number = 0;
	cout << "�Ѿ����" << endl;
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
	cout << "*****  1.�����ϵ��  *****" << endl;
	cout << "*****  2.��ʾ��ϵ��  *****" << endl;
	cout << "*****  3.ɾ����ϵ��  *****" << endl;
	cout << "*****  4.������ϵ��  *****" << endl;
	cout << "*****  5.�޸���ϵ��  *****" << endl;
	cout << "*****  6.�����ϵ��  *****" << endl;
	cout << "*****  0.�˳�ͨѶ¼  *****" << endl;
	cout << "**************************" << endl;
}