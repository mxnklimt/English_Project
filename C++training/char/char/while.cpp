#include "iostream"
using namespace std;

int while_ex()
{
	int num = 0;

	while ((num >=0) && (num < 10))
	{
		cout << "num = " << num << endl;
		num++;
	}

	system("pause");
	return 0;

}