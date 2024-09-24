#include "iostream"
using namespace std;

int bubble()
{
	int a[9] = { 8,9,5,3,4,5,0,1,2 };
	//9个元素，a[0]-a[9]
	for (int i = 0; i < 9 - 1; i++)
	{
		for (int j = 0; j < 9 - 1 - i;j++)
		{
			
			if (a[j + 1] < a[j])
			{
				int temp;
				temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
			}
		}
	}
	//冒泡排序
	//8,9,5,3,4,5,0,1,2
	//第一步，相邻两个元素比较大小，9个元素总共要比较8次，然后最大的元素就会在a[8]中
	//随后，在前8个元素中比较大小，比较7次，找出第二大的元素
	//所以，9个元素，要确定8个的位置，就可以确定全部的位置，总共要确定8次，对应的是i
	//j对应的是哪两个元素比较
	for (int i = 0; i < 9; i++)
	{
		cout << a[i]<<endl;
	}
	cout << sizeof(a) << endl;
	system("pause");
	return 0;

}