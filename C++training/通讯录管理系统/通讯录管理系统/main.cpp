#include "iostream"
using namespace std;
#include"my_struct.h"
#include"my_function.h"
int main()
{
	AddressBook abs;
	//初始化通讯录结构体变量
	abs.number = 0;


	while (true)
	{
		show();
		int input;
		cin >> input;
		switch (input)
		{
			case 1:
				AddPerson(&abs);
				break;
			case 2:
				ShowAddressBook(&abs);
				break;
			case 3:
				DeleteData(&abs);
				break;
			case 4:
				FindPeople(&abs);
				break;
			case 5:
				Revise(&abs);
				break;
			case 6:
				ResetData(&abs);
				break;
			case 0:
				cout << "thanks" << endl;
				return 0;
				break;

		default:
			break;
		}
		system("cls");
	}

	system("pause");
	return 0;

}