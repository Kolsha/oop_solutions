#ifndef TESTS_H
#define TESTS_H
#include <vector>
#include "gtest/gtest.h"
#include "object_pool.h"


class Point {

public:
    int m_x, m_y;
    Point()
        : m_x(0), m_y(0) {}

    Point(int x, int y)
        : m_x(x), m_y(y) {}
    ~Point(){}
};

TEST(Object_Pool, PointTest) {

    ObjectPool<Point> pp(2);

    ASSERT_EQ(pp.size(), 2);

    int x = rand();
    int y = rand() + x;
    Point &p1 = pp.alloc(x, y);
    ASSERT_EQ(p1.m_x, x);
    ASSERT_EQ(p1.m_y, y);
    Point &p2 = pp.alloc();
    ASSERT_EQ(p2.m_x, 0);
    ASSERT_EQ(p2.m_y, 0);
}

TEST(Object_Pool, ThrowTest) {

    ObjectPool<Point> pp(1);
    auto p1 = pp.alloc();
    EXPECT_THROW(pp.alloc(), OutOfLimitException);

}


#endif // TESTS_H
