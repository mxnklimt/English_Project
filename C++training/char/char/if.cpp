#include "iostream"
using namespace std;

int score_ex()
{
	int a = 0;
	cin >> a;
	if (a > 600)
	{
		cout << "normal" << endl;
	}
	else if (a > 500)
	{
		cout << "//" << endl;
	}
	else
	{
		cout << "what are you doing" << endl;

	}
	system("pause");
	return 0;

}