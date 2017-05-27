#ifndef BUDGET_H
#define BUDGET_H
#include <vector>
#include <unordered_map>
#include "spendrow.h"

struct BudgetRow{
    std::vector< std::string > cats;
    double budget;
};

struct CalculatedRow{
    std::string title;
    double expected_val;
    double real_val;
};

class Budget
{
private:
    static const char cat_del = '+';
    std::unordered_map< size_t, BudgetRow > rows;
public:
    Budget() = default;
    Budget(const std::string& cats, const double budget);
    size_t insert_row(const std::string& cats, const double budget);
    bool get_row(const size_t id, BudgetRow& row);

    void del_row(const size_t id){
        if(id > 0){
            rows.erase(id - 1);
        }
    }
    std::vector< CalculatedRow > calculate(SpendRow& Costs,
                                         const time_t start = 0, const time_t end = 0);
};

#endif // BUDGET_H
