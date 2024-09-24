#include "iostream"
#include "ctime"
using namespace std;

int random_ex()
{
	srand((unsigned int)time(NULL));
	//利用系统时间生成随机数
	//需要#include "ctime"
	int num = rand()%100;
	cout << num << endl;

	system("pause");
	return 0;

}