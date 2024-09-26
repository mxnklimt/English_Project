#include <iostream>
//静态成员变量static
//1.所有对象都共享同一份数据!
//2.编译阶段就分配内存
//3.类内声明，类外初始化操作

//静态成员函数
//1.所有对象共享同一个函数
//2.静态成员函数只能访问静态成员变量
class Person
{
    public:
    static int m_A;
    //1.所有对象都共享同一份数据!
    //2.编译阶段就分配内存
    //3.类内声明，类外初始化操作


    //静态成员函数
    static void func()
    {
        m_A = 250;
        //静态成员函数可以访问静态成员变量，不可以访问非静态成员变量
        //因为无法区分到底是哪个对象的非静态成员变量
        std::cout << "static void func调用" << std::endl;
    }
};
int Person::m_A = 100;
//类外初始化
void test00()
{
    Person p;
    std::cout << p.m_A << std::endl;
}
void test01()
{
    Person p;
    std::cout << p.m_A << std::endl;
    Person p2;
    p2.m_A = 200;
    std::cout << p.m_A << std::endl;

}
void test02()
{
    //1.通过对象访问静态成员函数
    Person p;
    p.func();

}
void test03()
{

    Person::func();
}
//类外 
int main()
{
    //test00(); 
    //test01();
    //test02();
    
    test03();
    std::cout << Person::m_A << std::endl;
    std::cout << "Hello world!" << std::endl;
}
