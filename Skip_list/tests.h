#ifndef TESTS_H
#define TESTS_H
#include <vector>
#include "gtest/gtest.h"
#include "skip_list.h"


TEST(SkipListTest, RandTest) {

    SkipList<int, int> list;
    std::vector<int> list_real;
    for(int i = 0; i < 100; i++)
    {
        int tmp = rand() % 100;
        std::pair<const int, int> val_to_insert(i, tmp);
        list.insert(val_to_insert);
        list_real.push_back(tmp);
    }
    for(int i = 0; i < 100; i++)
    {
        auto it = *(list.find(i));
        ASSERT_EQ(it.second, list_real[i]);
    }

}


#endif // TESTS_H
