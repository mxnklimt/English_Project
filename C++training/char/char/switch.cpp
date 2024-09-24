#include "iostream"
using namespace std;

int switch_ex()
{
	int score = 0;
	cin >> score;
	switch (score)
	{
	case 1:
		cout << "a piece of shit" << endl;
		break;
	case 2:
		cout << "poor" << endl;
		break;
	case 3:
		cout << "ok" << endl;
		break;
	default:
		cout << "Ä¬ÈÏºÃÆÀ" << endl;
		break;
	}
	system("pause");
	return 0;

}