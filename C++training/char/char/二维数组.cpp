#include "iostream"
using namespace std;

int arr_ex()
{
	int arr[2][3] =
	{
		{1,2,3},
		{4,5,6}
	};
	cout << "��ά����ռ�����ڴ棺 " << sizeof(arr) << endl;
	cout << "��ά����һ��ռ�õ��ڴ棺 " << sizeof(arr[0]) << endl;
	
	cout << "��ά������׵�ַ�� " << arr << endl;
	cout << "��ά����ڶ��е�ַ" << arr[1] << endl;
	cout << "��ά������׵�ַ�� " << (long long)arr << endl;
	cout << "��ά����ڶ��е�ַ" << (long long)arr[1] << endl;

	system("pause");
	return 0;

}