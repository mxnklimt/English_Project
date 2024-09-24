#include <iostream>
#include<circlepoint.h>

int main()
{
    circle c1;
    c1.r = 10;
    std::cout << "圆的周长为： " << c1.calculateZC() << std::endl;
    std::cout << "Hello world!" << std::endl;

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

