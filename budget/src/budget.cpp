#include <iostream>
#include "budget.h"
#include "utils.h"
using namespace std;

Budget::Budget()
{

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

size_t Budget::insert_row(const string cats, const double budget){

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

    for (const auto& kv : row.cats) {
        std::cout << kv  << std::endl;
    }

}
vector< std::string > Budget::calculate(SpendRow& Costs,
                                        const time_t start, const time_t end){
    vector< std::string > result;
    double all_sum = Costs.get_sum("", start, end);
    double other_sum = 0;
    if(all_sum <= 0){
        return result;
    }
    for (const auto& kv : rows) {
        BudgetRow row = kv.second;
        double tmp_sum = 0;
        string title;
        for (const auto& kv : row.cats) {
            tmp_sum += Costs.get_sum(kv, start, end);
            title += kv + " + ";
        }

        double percent = (tmp_sum / row.budget) * 100;

        other_sum += tmp_sum;
        title = title.substr(0, title.length() - 2);
        title += to_string(row.budget) + " ";
        title += to_string(tmp_sum) + " ";
        title += to_string(percent) + "%";
        result.push_back(title);


    }
    other_sum = all_sum - other_sum;
    string title = "Other 0 " + to_string(other_sum);
    result.push_back(title);

    return result;
}
