#include <iostream>
#include "spendrow.h"
#include "utils.h"
using namespace std;

SpendRow::SpendRow(const string cat, const time_t ts, const double sum)
{
    insert_buy(cat, ts, sum);
}


bool SpendRow::insert_buy(const string cat, const time_t ts,  const double sum){

    if(sum <= 0 || ts == 0){
        return false;
    }

    if(!cat.empty()){
        std::string new_cat, cur_cat = cat;
        auto it = cat.find_first_of(cat_del);
        if(it != string::npos){
            cur_cat = cat.substr(0, it);
            new_cat = cat.substr(++it);
        }

        if(child == nullptr){
            child = new unordered_map<std::string, SpendRow>;

        }

        if(child == nullptr){
            return false;
        }

        trim(cur_cat);
        trim(new_cat);

        auto map_it = child->find(cur_cat);
        if(map_it == child->end()){
            SpendRow tmp;
            bool res_tmp = tmp.insert_buy(new_cat, ts, sum);
            child->insert({cur_cat, std::move(tmp)});
            return res_tmp;

        }else{
            SpendRow tmp  = child->at(cur_cat);
            return tmp.insert_buy(new_cat, ts, sum);
        }

    }else{
        buys[ts] += sum;
        return true;
    }

    return false;
}

double SpendRow::get_sum(const std::string cat, const time_t start,
                         const time_t end){
    if(!cat.empty()){
        std::string new_cat, cur_cat = cat;
        auto it = cat.find_first_of(cat_del);
        if(it != string::npos){
            cur_cat = cat.substr(0, it);
            new_cat = cat.substr(++it);
        }

        if(child == nullptr){
            return -1;
        }


        trim(cur_cat);
        trim(new_cat);


        auto map_it = child->find(cur_cat);
        if(map_it != child->end()){
            SpendRow tmp  = child->at(cur_cat);
            return tmp.get_sum(new_cat, start, end);
        }
        return -1;

    }else{
        double all_sum = 0;
        for (const auto& kv : buys) {
            if((kv.first > end && end > 0) ||
                    (kv.first < start && start > 0)){
                continue;
            }
            all_sum += kv.second;
        }

        if(child != nullptr){
            for (const auto& kv : *child) {
                SpendRow tmp = kv.second;
                all_sum += tmp.get_sum(cat, start, end);
            }
        }

        return all_sum;
    }

    return -1;
}
