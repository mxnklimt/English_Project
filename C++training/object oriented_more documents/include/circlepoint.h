#ifndef __pointcircle_h
#define __pointcircle_h
class point
{
    int x;
    int y;
    public:
    int setX(int X);
    int setY(int Y);
    int getX();
    int getY ();
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
    double calculateZC();
    void setcenter(point center1);
    float getd(point* center2);
};
#endif