#pragma once
#include <vector>
#include "gtest/gtest.h"
#include "multi_vector.h"


TEST(MultiVecTest, CommonTest) {

    multi_vector<double, 2> a({2, 2}), c({1, 1});

    ASSERT_EQ(a[0], 2);
    ASSERT_EQ(a[1], 2);

    ASSERT_TRUE(a != c);
    ASSERT_TRUE(c < a);

    multi_vector<double, 2> d = (a + c) * 3;
    multi_vector<double, 2> d_r({3, 3});
    d_r *= 3;
    ASSERT_TRUE(d == d_r);

    a += c;
    a *= 3;

    ASSERT_TRUE(a == d_r);

    const size_t to_sum_dim = 33;
    multi_vector<int, to_sum_dim> to_sum;
    int to_sum_int = rand() % 100;
    to_sum += to_sum_int;
    for(auto &row: to_sum){
        ASSERT_EQ(row, to_sum_int);
    }

    to_sum -= to_sum_int;

    int scalar_res = to_sum.scalar_multiply(to_sum);
    ASSERT_EQ(scalar_res, 0);

}

TEST(MultiVecTest, 2DTest) {

    multi_vector<double, 2> a({2, 2}), c({1, 1});
    multi_vector<double, 2> d = (a + c) * 3;
    multi_vector<double, 2> d_r({9, 9});
    ASSERT_TRUE(d == d_r);

}

TEST(MultiVecTest, TransformTest) {

    multi_vector<double, 2> a({2, 2}), tr_res({4, 0});
    Matrix<double, 2> basis;
    basis[0] = {1, 1};
    basis[1] = {-1, 1};


    a.transform(basis);

    ASSERT_TRUE(a == tr_res);

}

