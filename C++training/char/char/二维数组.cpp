#include "iostream"
using namespace std;

int arr_ex()
{
	int arr[2][3] =
	{
		{1,2,3},
		{4,5,6}
	};
	cout << "二维数组占用总内存： " << sizeof(arr) << endl;
	cout << "二维数组一行占用的内存： " << sizeof(arr[0]) << endl;
	
	cout << "二维数组的首地址： " << arr << endl;
	cout << "二维数组第二行地址" << arr[1] << endl;
	cout << "二维数组的首地址： " << (long long)arr << endl;
	cout << "二维数组第二行地址" << (long long)arr[1] << endl;

	system("pause");
	return 0;

}