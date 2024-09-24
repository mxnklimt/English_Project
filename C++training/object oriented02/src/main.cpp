#include <iostream>


class Person
{
    public:
    int age;
    Person()
    //构造函数
    //没有返回值，不用写void
    //构造函数可以有参数，可以发生重装载
    //创建对象的时候，构造函数会自动调用，而且只调用一次
    {
        std::cout << std::endl;
        std::cout << "Person构造函数的调用" << std::endl;
    }

    ~Person()
    //析构函数
    //没有返回值不写void
    //析构函数不可以有参数
    //对象在销毁前会调用析构函数，而且只会调用一次
    {
        std::cout << "Person析构函数的调用" << std::endl;
    }
    Person(int a)
    {
        age = a;
        std::cout << std::endl;
        std::cout << "析构函数可以有参" << std::endl;
    }
    Person(const Person &p)
    //拷贝构造函数
    {
         //将传入的人身上的所有属性，拷贝到我身上
        age = p.age;
    }

};

void xg_test()
{
    Person p1;
    //栈上的数据，test执行完毕后释放这个对象
}
int main()
{
    //1括号法
    Person p;//第一次创建对象时候，会自动调用构造函数
    //调用默认构造函数时，不要加()
    

    xg_test();
    Person p2(10);//有参构造函数
    Person p3(p2);
    std::cout << "p2 = "<< p2.age << std::endl;
    //----------------------------------------------------
    //2显示法
    Person p1;
    Person p2 = Person(10);
    Person p3 = Person(p2);

    Person(10);//匿名对象
    //当上面这行结束后，系统会立即回收匿名对象
    //------------------------------------------------------
    //3显示法
    Person p4 = 10;//相当于Person p4 = Person(10)
    Person p5 = p4;
     
}
