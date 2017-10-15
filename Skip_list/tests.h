#ifndef TESTS_H
#define TESTS_H
#include <vector>
#include "gtest/gtest.h"
#include "skip_list.h"

using namespace std;

TEST(SkipListTest, RandTest) {

    SkipList<int, int> list;
    std::vector<int> list_real;
    for(int i = 0; i < 100; i++)
    {
        int tmp = rand() % 100;
        std::pair<const int, int> val_to_insert(i, tmp);
        list.insert(val_to_insert);
        list_real.push_back(tmp);
        ASSERT_EQ(list[i], tmp);
    }
    ASSERT_EQ(list.size(), (size_t)100);
    for(int i = 0; i < 100; i++)
    {
        auto it = *(list.find(i));
        ASSERT_EQ(it.second, list_real[i]);
    }

}

TEST(SkipListTest, IteratorTest) {
    SkipList<int, std::string> list;
    std::string test = "tests";
    for(size_t i = 0; i < 10; i++){
        std::pair<const int, ::string> tmp = {0, test};
        list.insert(tmp);
    }

    for(auto &row:list){
        ASSERT_EQ(row.second, test);
    }

    list.erase(list.begin());

}


#endif // TESTS_H
