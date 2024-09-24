#include"iostream"
#include"myfunction.h"

void sitetest()
{
    int a = 10;
    int &b = a;//创建引用(给a变量起了个别名b)
    //引用必须要初始化，初始化后不可更改指向的对象
    //引用的本质就是一个指针常量
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    b = 100;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

}
void swap(int *a , int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;

}
void siteswap(int &a , int &b)
//利用引用来传递数据
{
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
    int temp = a;
    a = b;
    b = temp;
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;
}
int& sitetestlow()
//函数返回值是一个引用
//如果函数的返回值是一个引用，这个函数调用可以作为左值
{
    static int a = 10;
    //不加static就是局部变量，不能返回
    return a;
}
void sitetest01()
{
    int &ref = sitetestlow();
    std::cout << "ref = " << ref << std::endl;
    sitetestlow() = 1000;
    std::cout << "ref = " << ref << std::endl;
}
void siteconst00()
{
    const int& ref = 10;
    //编译器将代码修改为 temp =10；const int &ref = temp；

}

void siteconst01(const int& a)
{
    std::cout << a << std::endl;
}

int func00(int a , int b = 10 , int c = 100)
//其中有默认参数的形参要放在后面
//如果一个函数声明有默认参数，那他的函数实现就不能有默认参数
//函数声明和函数本身只能有一个有默认参数
{
    return a+b+c;
}
void func00(int a , int b = 10 )
{

}
//函数重载（可以让函数名相同，提高复用性）
//1.要在同一个作用域下
//2.函数名称相同
//3.函数参数类型不同，或者个数不同，或者顺序不同
//函数返回值类型不可以作为函数重载的条件
void func00(int a)
{   

}
void func00(double a)
{

}

void func01(int a, int)
//占位函数，调用函数的时候必须要填补该位置，如func01()

{
    
}
