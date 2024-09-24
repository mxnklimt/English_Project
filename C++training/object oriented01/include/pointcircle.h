#include<string>
#include<math.h>
#ifndef __pointcircle_h
#define __pointcircle_h

class point;
class circle;
#define PI 3.14


//class 的声明


class point
{
    int x;
    int y;
    public:
    int setX(int X)
    {
        x = X;
    }
    int setY(int Y)
    {
        y = Y;
    }
    int getX()
    {
        return x;
    }
    int getY ()
    {
        return y;
    }
};

class circle
{
    
    //访问权限

public:

    //属性
    int r;
    int x;
    int y;
    point center;
    //因为class point；的前向声明
    //前向声明允许你告诉编译器某个类的存在，而不必提供完整的定义。
    //前向声明只允许你声明指向未定义类型的指针或引用，而不能直接使用未定义的类型作为对象。
    double calculateZC()
    {
        return 2*PI*r;
    }
    void setcenter(point center1)
    {
        center = center1;
    }
    float getd(point* center2)
    {
        float a;
        a = (pow((center.getX()-center2->getX()),2) )+(pow((center.getY()-center2->getY()),2));
        a = sqrt(a);
    }
};








#endif