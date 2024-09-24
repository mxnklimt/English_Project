#include <iostream>
#include "myfunction.h"
//内存的分区
//代码区
//全局区： 全局变量，静态变量，常量（字符串常量+全局常量）
//栈区：函数参数，局部变量，局部常量  

//堆区:由程序员分配和释放，若程序员不释放，程序结束后由操作系统回收

//代码区是共享的，对频繁被执行的程序，只需要在内存中有一份代码即可
//代码区是只读的，防止程序意外修改

//不要返回局部变量的地址，栈区开辟的数据由编译器自动释放


//src：源代码(source)
//lib：库文件(library)
//include：头文件
//
int g_a = 10;
int g_b = 10;//全局变量

const int c_g_a = 10;//全局常量

int *func()
{
    int a = 10;
    return &a;//返回局部变量地址，该局部变量存放在栈区，栈区数据在函数执行完后自动释放
}
int *newdata()
{
    int *p = new int(10);//指针接收这块内存
    //利用new可以将数据开辟到堆区,存放数据10
    //指针本质是局部变量，是放在栈上，但是这个数据是在堆区的
    return p;
}
void newarray()
{
     int *arr = new int[10];//在堆上创建数组
     for(int i = 0;i < 10; i++)
     {
        arr[i] = i+100;

     }
     for(int i = 0;i < 10; i++)
     {
        std::cout << arr[i] << std::endl;        
     }
     delete[] arr;//释放堆上的数组 
     for(int i = 0;i < 10; i++)
     {
        std::cout << arr[i] << std::endl;        
     }
}
int main()
{
    int a = 10;
    int b = 20;//局部变量

    static int s_a = 10;
    static int s_b = 10;//静态变量
    
    const int c_l_a = 10;
    const int c_l_b = 10;//局部常量

    std::cout << std::endl;
    std::cout << "局部变量a的地址：\t" << &a << std::endl;
    std::cout << "局部变量b的地址：\t" << &b << std::endl;
    std::cout << "全局变量g_a的地址：\t" << &g_a << std::endl;
    std::cout << "全局变量g_b的地址：\t" << &g_b << std::endl;
    std::cout << "静态变量s_a的地址：\t" << &s_a << std::endl;
    std::cout << "静态变量s_b的地址：\t" << &s_b << std::endl;
    std::cout << "字符串常量的地址：\t" << &"mxn" << std::endl;
    std::cout << "全局常量的地址：\t" << &c_g_a << std::endl;
    int *p = newdata();
    std::cout << "newdata：\t" << *p << std::endl;
    delete p;
    std::cout << "newdata：\t" << *p << std::endl;//内存已经被delete释放，再访问就是非法操作
    newarray();
    // 局部变量a的地址： 0x62fe1c
    // 局部变量b的地址： 0x62fe18
    // 全局变量a的地址： 0x403010
    // 全局变量b的地址： 0x403014
    // 静态变量s_a的地址： 0x403018
    // 静态变量s_a的地址： 0x40301c
    // 可见（全局变量/静态变量）和局部变量所放的区域不一样
    //vscode ctrl+/ 注释快捷键
    sitetest();
    siteswap(a , b);
    sitetest01();
    //system("pause");
    return 0;

}

