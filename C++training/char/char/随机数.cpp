#include "iostream"
#include "ctime"
using namespace std;

int random_ex()
{
	srand((unsigned int)time(NULL));
	//����ϵͳʱ�����������
	//��Ҫ#include "ctime"
	int num = rand()%100;
	cout << num << endl;

	system("pause");
	return 0;

}