#include "iostream"
using namespace std;


void swap_p(int *p1,int *p2)
{
	int temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}

int swap_p_main()
{
	int a = 10;
	int b = 20;
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "地址传递后"<< endl;

	swap_p(&a, &b);
	//地址传递可以改变实参的值

	cout <<"a = " << a << endl;
	cout <<"b = " << b << endl;



	system("pause");
	return 0;

}