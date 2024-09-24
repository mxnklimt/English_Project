#include <iostream>
//初始化列表
class Person
{
public:
// //1.传统初始化
//     Person(int a, int b, int c)
//     {
//         m_a = a;
//         m_b = b;
//         m_c = c;
//     }

//2.初始化列表初始化属性
    Person(int a, int b, int c):m_a(a ),m_b(b),m_c(c)
    {

    }
    int m_a;
    int m_b;
    int m_c;
};

void test00()
{
    Person p(10,20,30);
    std::cout << "m_a=" << p.m_a << std::endl;
    std::cout << "m_b=" << p.m_b << std::endl;
    std::cout << "m_c=" << p.m_c << std::endl;
}
int main(int argc, char *argv[])
{
    test00();
    std::cout << "Hello world!" << std::endl;
}
