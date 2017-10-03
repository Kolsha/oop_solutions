#ifndef TESTS_H
#define TESTS_H
#include <vector>
#include "gtest/gtest.h"
#include "merging_iterator.h"

class Comparator
{
public:
    int operator () (int a, int b)
    {
        return a > b ? 1: 0;
    }
    int operator () (double a, double b)
    {
        return a > b ? 1: 0;
    }

};

TEST(MergeIterator, FromOOP) {

    std::vector<int> first = {1, 2, 7};
    std::vector<int> second = {3, 5, 8};
    std::vector<int> fird = {6, 10, 12};
    std::vector<int> res(9, 0);
    std::vector<int> res_real = {1, 2, 3, 5, 6, 7, 8, 10, 12};

    Comparator cmp;

    my_merge(
    {first.begin(), second.begin(), fird.begin()},
    {first.end(), second.end(), fird.end()},
                res.begin(),
                cmp
                );
    for(size_t i = 0; i < res.size(); i++){
        ASSERT_EQ(res[i], res_real[i]);
    }
}


#endif // TESTS_H
