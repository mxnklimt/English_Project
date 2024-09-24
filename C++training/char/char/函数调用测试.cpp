#include "iostream"
using namespace std;
#include"swap.h"
#include"BubbleSort.h"
#include"my_function.h"
int test()
{
	//int a = 10;
	//int b = 20;
	//show(a);

	int arr[10] = { 7,5,6,9,4,1,2,6,4,8};
	int len = sizeof(arr) / sizeof(arr[0]);

	BubbleSort(arr, len);
	//for (int i = 0; i < len; i++)
	//{
	//	cout << arr[i] << endl;
	//}
	PrintArray(arr, len);


	system("pause");
	return 0;

}