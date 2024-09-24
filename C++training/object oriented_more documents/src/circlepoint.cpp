#include<string>
#include<math.h>
#include<circlepoint.h>

class point;
class circle;
#define PI 3.14


//class 的声明



  
    int point::setX(int X)
    {
        x = X;
    }
    int point::setY(int Y)
    {
        y = Y;
    }
    int point::getX()
    {
        return x;
    }
    int point::getY ()
    {
        return y;
    }



    
   
 
    //因为class point；的前向声明
    //前向声明允许你告诉编译器某个类的存在，而不必提供完整的定义。
    //前向声明只允许你声明指向未定义类型的指针或引用，而不能直接使用未定义的类型作为对象。
    double circle::calculateZC()
    {
        return 2*PI*r;
    }
    void circle::setcenter(point center1)
    {
        center = center1;
    }
    float circle::getd(point* center2)
    {
        float a;
        a = (pow((center.getX()-center2->getX()),2) )+(pow((center.getY()-center2->getY()),2));
        a = sqrt(a);
    }








