#include "iostream"
using namespace std;

int cin_ex()
{
	cout << "������α�����ֵ" << endl;
	int a = 0;

	cin >> a;
	//��ֵ���
	cout << "���ͱ���a = " << a<<endl;
	
	//ctrl+k+c ѡ�����ݱ�Ϊע��

	string str2 = "xmnklimt";
	cout << str2 << endl;
	cin >> str2;
	cout << "str2= " << str2 << endl;

	bool flag = false;
	cin >> flag;
	cout << "bool flag = " << flag << endl;

	 

	system("pause");
	return 0;

}