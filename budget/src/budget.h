#ifndef BUDGET_H
#define BUDGET_H
#include <vector>
#include <unordered_map>
#include <spendrow.h>
#include <spendrow.h>

struct BudgetRow{
    std::vector< std::string > cats;
    double budget;
};

class Budget
{
private:
    static const char cat_del = '+';
    std::unordered_map< size_t, BudgetRow > rows;
public:
    Budget();
    size_t insert_row(const std::string cats, const double budget);
    bool get_row(const size_t id, BudgetRow& row);

    void del_row(const size_t id){
        if(id > 0){
            rows.erase(id - 1);
        }
    }
    std::vector< std::string > calculate(SpendRow& Costs,
                                         const time_t start = 0, const time_t end = 0);
};

#endif // BUDGET_H