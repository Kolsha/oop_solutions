#include <iostream>
#include "gtest/gtest.h"
#include "../src/spendrow.h"
using namespace std;

TEST(BudgetTest, SpendRowTest) {

    SpendRow test("Auto:BMW", 12345678, 500);
    test.insert_buy("Moto:Kawasaki:Ninja", 12344, 300);
    test.insert_buy("Moto:Kawasaki:ZZR 400", 12344, 100);

    double all_sum = test.get_sum("Moto:Kawasaki:Ninja", 10, 12344);
    ASSERT_EQ(all_sum, 300);

    all_sum = test.get_sum("Moto:Kawasaki:Ninja", 10, 111);
    ASSERT_NE(all_sum, 300);
    ASSERT_EQ(all_sum, 0);

    all_sum = test.get_sum("", 0, 0);

    ASSERT_EQ(all_sum, 900);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
