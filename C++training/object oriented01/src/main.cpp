#include <iostream>
#include <myfunction.h>
#include<pointcircle.h>

int main()
{
    circle c1;
    c1.r = 10;
    std::cout << "圆的周长为： " << c1.calculateZC() << std::endl;
    std::cout << "Hello world!" << std::endl;

    student s1;
    s1.name = "jx";
    s1.id  = 1;
    s1.showstudent();

    student s2;
    s2.name = "jd";
    s2.id  = 2;
    s2.showstudent();

    s2.setname("hi");
    s2.setid(123);
    s2.showstudent();

    people p1;
    //p1.func(); 在func()是public的时候可以访问，是private则不能访问

    cube cube1;

    cube1.setl(10);
    cube1.setw(10);
    cube1.seth(10);
    std::cout << cube1.getl() << std::endl;
    std::cout << cube1.getw() << std::endl;
    std::cout << cube1.geth() << std::endl;
    std::cout << "V = "<< cube1.getV() << std::endl;
    std::cout << "S = "<< cube1.getS() << std::endl;
    

    point point0;
    point0.setX(10);
    point0.setY(20);
    std::cout << "X = " << point0.getX() << std::endl;
    std::cout << "Y = " << point0.getY() << std::endl;

    point point1;
    point1.setX(30);
    point1.setY(30);
    c1.setcenter(point1);
    std::cout << "d= "<<c1.getd(&point0) << std::endl;

}
