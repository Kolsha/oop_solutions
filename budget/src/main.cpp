#include <iostream>
#include "spendrow.h"
#include "budget.h"
using namespace std;


int main(int argc, char *argv[])
{

    SpendRow test("Auto:BMW", 12345678, 500);
    test.insert_buy("Moto:Kawasaki:Ninja", 12344, 300);
    test.insert_buy("Moto:Kawasaki:ZZR 400", 12344, 100);
    test.insert_buy("Moto:Racer:Nitro 200", 12344, 30);
    test.insert_buy("Moto:Racer:Nitro 250", 12344, 40);
    test.insert_buy("Xioami:Notebook:Air 13.3", 12344, 30);
    test.insert_buy("Xioami:Phone:Mi 5S", 12344, 19);
    Budget MyBudget;
    MyBudget.insert_row("Auto:BMW + Moto:Kawasaki", 1000);
    MyBudget.insert_row("Moto:Racer", 1000);
    MyBudget.insert_row("Xioami", 1000);
    vector<CalculatedRow> res = MyBudget.calculate(test);
    for (const auto& kv : res) {
        CalculatedRow row = kv;
        cout << row.title << "\t" << row.expected_val <<
                "\t" << row.real_val << "\t" <<
                (row.real_val / row.expected_val) * 100 << "%"<< endl;
    }
    return 0;
}
