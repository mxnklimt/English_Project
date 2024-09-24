#include "iostream"
using namespace std;

int arrandp_ex()
{
	int arr[10] = { 0,1,2,3,4,5,6,7,8,9 };
	int* p = arr;
	//arr就是数组的首地址
	cout << "利用指针访问数组的首地址： " << *p << endl;
	p++;
	cout << "利用指针访问数组第二个元素： " << *p << endl;

	
	//利用指针遍历数组
	p--;
	for (int i = 0; i < 10; i++)
	{
		cout << *p << endl;
		p++;
	}

	system("pause");
	return 0;

}