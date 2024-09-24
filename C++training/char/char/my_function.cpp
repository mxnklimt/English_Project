#include"iostream"
using namespace std;
#include"my_function.h"

void PrintArray(int* arr, int len)
{
	for (int i = 0; i < len; i++)
	{
		cout << "arr[" << i << "] = " << arr[i] << endl;
	}
}
