#include <string>
//#include <iostream>

#ifndef __myfunction_h
#define __myfunction_h



// //函数声明：显然应该放在头文件中，前面很清楚。
// 类定义、结构定义：用函数定义的逻辑想，似乎不能放在头文件中。但它应该放在头文件。第一，每个cpp文件应该有一个定义，在编译的时候编译器才知道怎么为对象分配空间。其次，类型定义不会在内存上分配空间。
// 模板函数：编译器必须在编译的时候根据函数模板实例化对应的函数，所以应该放在头文件。
// 内联函数：编译期间被插到调用位置，所以也要放在头文件。
// 函数定义：不要！C++规定一个程序同签名的函数只能有一个定义。如果你把函数定义放在头文件件，并且同一个程序的多个cpp文件包含了该头文件，这样，在链接的时候会发现多个定义版本，链接报错。
// 变量定义：不要！与上面类似，被多个文件包含的时候会出现多次定义同一个变量，链接错误。但是，static变量和extern变量可以，以及宏定义的常量，因为这些在多个文件出现并不会出错。                   
// 原文链接：https://blog.csdn.net/chen_z_p/article/details/105352547



// class point;
// class circle;


// //class 的声明

// class circle
// {
    
//     //访问权限

// public:

//     //属性
//     int r;
//     int x;
//     int y;
//     point *center;
//     //因为class point；的前向声明
//     //前向声明允许你告诉编译器某个类的存在，而不必提供完整的定义。
//     //前向声明只允许你声明指向未定义类型的指针或引用，而不能直接使用未定义的类型作为对象。
//     double calculateZC()
//     {
//         return 2*PI*r;
//     }
//     void setcenter(point* center1)
//     {
//         center = center1;
//     }
//     int getd(point* center2)
//     {
//         return ((center->getX()-center2->getX())^2 )*((center->getY()-center2->getY())^2);
//     }
// };
// class point
// {
//     int x;
//     int y;
//     public:
//     int setX(int X)
//     {
//         x = X;
//     }
//     int setY(int Y)
//     {
//         y = Y;
//     }
//     int getX()
//     {
//         return x;
//     }
//     int getY ()
//     {
//         return y;
//     }
// };
//类中的属性和行为我们统一称为成员
//属性 成员属性 成员变量
//行为 成员函数 成员方法




class student
{

    public:

    std::string name;
    //没有using namespace需要std::string

    int id;
    void showstudent()
    {
        std::cout << "姓名： " << name << "学号： " << id << std::endl;

    }
    void setname(std::string a)
    {
        name = a;
    }
    void setid(int a)
    {
        id = a;
    }

};

class people
{
    public:
    std::string name;
    
    protected:
    int password;

    private:
    std::string history;

    private:
    
        void func()
        {
            name = "mxn";
            password = 453;
            history = "waht";
            //类内func可以访问三种属性
        }
    
} ;
//访问权限
//1.public  公共权限 成员 类内可以访问 类外可以访问
//2.protected 保护权限 成员 类内可以访问 类外不可以访问 子类可以访问父类
//3.private 访问权限 成员 类内可以访问 类外不可以访问 子类不可访问父类
//class和struct唯一的区别在于默认的访问权限不同，struct默认是公共，class默认是私有


class cube
{
    int l;
    int w;
    int h;
    public:
    void setl(int L)
    {
        l = L;
    }
    void setw(int W)
    {
        w = W;
    }
    void seth(int H)
    {
        h = H;
    }
    int getl()
    {
        return l;
    }
    int getw()
    {
        return w;
    }
    int geth()
    {
        return h;
    }
    int getV()
    {
        return l*w*h;
    }
    int getS()
    {
        return 2*l*w + 2*l*h + 2*w*h ;
    }
    
};








#endif