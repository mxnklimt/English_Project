#include "iostream"
using namespace std;

int specialchar()
{
	cout << "what\n";
	// \n写在引号内实现了换行，和<<endl效果类似

	cout << "\\" << endl;
	//打两个反斜杠，才能输出一个反斜杠

	cout << "aaa\tbbb" << endl; 
	//输出aaa     bbb   ，中间五个空格
	cout << "aaaa\tbbb" << endl;
	//输出aaaa    bbb   ，中间四个空格
	// \t 水平制表符 占用8个位置
	//一般是用来对齐，整齐输出数据

	system("pause");
	return 0;

}