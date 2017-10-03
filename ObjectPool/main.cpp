#include <iostream>
#include "object_pool.h"

using namespace std;

class Point {

public:
    int m_x, m_y;
    /*Point()                // конструктор без параметров
        : m_x(0), m_y(0) {}
        */
    Point(int x, int y)    // конструктор с параметрами
        : m_x(x), m_y(y) {}
    ~Point(){
        cout << "dest of point" << endl;
    }
};


int main(int argc, char *argv[])
{
    ObjectPool<Point> pp(10);     // выделена память под 10 объектов Point

    Point &p1 = pp.alloc(11, 13);       // сработал конструктор Point()
    cout << p1.m_x << " : " << p1.m_y << endl;
    Point &p2 = pp.alloc(0, 1);
    cout << p2.m_x << " : " << p2.m_y << endl;
    pp.free(p1);
    p1 = pp.alloc(133, 33);       // сработал конструктор Point()
    cout << p1.m_x << " : " << p1.m_y << endl;
    pp.free(p1);
    cout << "free end" << endl;


    // std::forward;
    return 0;
}
