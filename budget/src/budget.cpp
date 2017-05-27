#include <iostream>
#include "budget.h"
#include "utils.h"
using namespace std;

Budget::Budget(const string &cats, const double budget)
{
    insert_row(cats, budget);
}


inline bool Budget::get_row(const size_t id, BudgetRow& row){
    if(id > 0){
        try{
            row = rows.at(id - 1);
            return true;
        }
        catch(...){
        }
    }

    return false;
}


size_t Budget::insert_row(const string &cats, const double budget){

    if(cats.empty() || budget <= 0){
        return 0;
    }
    BudgetRow row;
    char del_cat = cat_del;
    if(explode(cats, del_cat, row.cats, true)){
        row.budget = budget;
        rows.insert({rows.size(), std::move(row)});
        return rows.size();
    }

    return 0;

}


std::vector<CalculatedRow> Budget::calculate(SpendRow& Costs,
                                             const time_t start, const time_t end){
    vector< CalculatedRow > result;
    double all_sum = Costs.get_sum(string(""), start, end);
    double other_sum = 0;
    if(all_sum <= 0){
        return result;
    }
    for (const auto& kv : rows) {
        BudgetRow row = kv.second;
        double tmp_sum = 0;
        CalculatedRow res_row;
        for (const auto& kv : row.cats) {
            tmp_sum += Costs.get_sum(kv, start, end);
            res_row.title += kv + " + ";
        }
        res_row.title = res_row.title.substr(0, res_row.title.length() - 2);

        res_row.expected_val = row.budget;
        res_row.real_val = tmp_sum;

        result.push_back(std::move(res_row));


    }
    CalculatedRow res_row = {"Other", 0, (all_sum - other_sum)};
    result.push_back(std::move(res_row));

    return result;
}
