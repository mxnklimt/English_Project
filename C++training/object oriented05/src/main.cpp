#include <iostream>
#include <string>
//类对象作为类成员

//类中的成员可以是另一个类的对象，我们称该成员为对象成员
//以下代码中，person p运行，由于Person中含有Phone类，所以先构造Phone类
class Phone
{
    public:
    std::string m_PName;
    Phone(std::string pname)
    {
        m_PName = pname; 
        std::cout << "Phone access"<< std::endl;
    }
    ~Phone()
    {
        std::cout << "Phone end"<< std::endl;
    }
};
class Person
{
    public:
    Person(std::string name,std::string pname):m_name(name) , m_Phone(pname)
    {
        std::cout << "Person access"<< std::endl;
    }
    std::string m_name;
    Phone m_Phone;
    ~Person()
    {
        std::cout << "Person end"<< std::endl;
    }
};
void test00()
{
    Person p("张三","Apple");
    std::cout << p.m_name << std::endl;
    std::cout << p.m_Phone.m_PName << std::endl;
}
int main()
{
    test00();
}
