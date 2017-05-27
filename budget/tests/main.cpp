#include <iostream>
#include "gtest/gtest.h"
#include "../src/spendrow.h"
#include "../src/budget.h"
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

}

TEST(BudgetTest, BudgetTest) {

    SpendRow test("Auto:BMW", 12345678, 500);
    test.insert_buy("Moto:Kawasaki:Ninja", 12344, 300);
    test.insert_buy("Moto:Kawasaki:ZZR 400", 12344, 100);
    test.insert_buy("Moto:Racer:Nitro 200", 12344, 30);
    test.insert_buy("Moto:Racer:Nitro 250", 12344, 40);
    test.insert_buy("Xioami:Notebook:Air 13.3", 12344, 30);
    test.insert_buy("Xioami:Phone:Mi 5S", 12344, 19);
    Budget MyBudget;
    MyBudget.insert_row(("Auto:BMW + Moto:Kawasaki"), 1000);
    MyBudget.insert_row("Moto:Racer", 1000);
    MyBudget.insert_row("Xioami", 1000);
    vector<CalculatedRow> res = MyBudget.calculate(test);
    for (const auto& kv : res) {
        CalculatedRow row = kv;
        if(row.title == "Auto:BMW + Moto:Kawasaki"){
            ASSERT_EQ(row.expected_val, 1000);
            ASSERT_EQ(row.real_val, 900);
        }

        else if(row.title == "Xioami"){
            ASSERT_EQ(row.expected_val, 1000);
            ASSERT_EQ(row.real_val, 49);
        }
        else if(row.title == "Moto:Racer"){
            ASSERT_NE(row.expected_val, 0);
            ASSERT_EQ(row.real_val, 70);
        }else{
            ASSERT_NE(row.real_val, 0);
        }

    }

}


TEST(BudgetTest, TZTest) {

    SpendRow test("Авто:Бензин", 24, 1400);
    test.insert_buy("Продукты:Мясо", 23, 500);
    test.insert_buy("Продукты:Хлеб", 23, 90.33);
    test.insert_buy("Общепит:Столовка", 24, 190);
    Budget MyBudget;
    MyBudget.insert_row("Авто", 6000);
    MyBudget.insert_row("Продукты + Общепит", 10000);
    MyBudget.insert_row("Алкоголь:Пиво", 900);
    vector<CalculatedRow> res = MyBudget.calculate(test);
    for (const auto& kv : res) {
        CalculatedRow row = kv;
        if(row.title == "Авто"){
            ASSERT_EQ(row.expected_val, 6000);
            ASSERT_EQ(row.real_val, 1400);
        }

        else if(row.title == "Продукты + Общепит"){
            ASSERT_EQ(row.expected_val, 10000);
            ASSERT_EQ(row.real_val, 780.33);
        }
        else if(row.title == "Алкоголь:Пиво"){
            ASSERT_NE(row.expected_val, 900);
            ASSERT_EQ(row.real_val, 0);
        }else{
            ASSERT_NE(row.real_val, 0);
        }

    }

}



int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
