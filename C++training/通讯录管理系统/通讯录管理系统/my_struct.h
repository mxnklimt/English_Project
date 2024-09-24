#ifndef MY_STRUCT_H
#define MY_STRUCT_H



#include"iostream"
#include"string"
using namespace std;
#define capacity 1000
struct Student
{
	string name;
	int level;
};
struct AddressBook
{
	struct Student personarray[capacity];
	int number;
};

#endif // !1