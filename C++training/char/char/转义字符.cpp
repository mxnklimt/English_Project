#include "iostream"
using namespace std;

int specialchar()
{
	cout << "what\n";
	// \nд��������ʵ���˻��У���<<endlЧ������

	cout << "\\" << endl;
	//��������б�ܣ��������һ����б��

	cout << "aaa\tbbb" << endl; 
	//���aaa     bbb   ���м�����ո�
	cout << "aaaa\tbbb" << endl;
	//���aaaa    bbb   ���м��ĸ��ո�
	// \t ˮƽ�Ʊ�� ռ��8��λ��
	//һ�����������룬�����������

	system("pause");
	return 0;

}