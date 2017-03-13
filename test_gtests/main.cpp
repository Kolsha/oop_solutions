#include <iostream>

#include "gtest/gtest.h"

using namespace std;


TEST(MyFirstTestCase, FirstTest) {
    string test = "test";
    ASSERT_EQ(test, "test");
}


TEST(MyFirstTestCase, SecondTest) {

    string test = "test";
    ASSERT_TRUE(test != "test");
}



int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

