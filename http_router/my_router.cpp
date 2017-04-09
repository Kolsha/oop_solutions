#include "my_router.h"
#include <algorithm>

#include <iostream>
#include <unordered_map>

using namespace std;

MyRouter::MyRouter()
{
    clear();
}

MyRouter::MyRouter(const std::string rule)
{
    clear();
    add_rule(rule);
}



bool MyRouter::check_rule(const string rule, std::vector<sRule> &res){

    res.clear();

    if(rule.length() < 1 || rule[0] != '/'){
        return false;
    }

    auto it_s = rule.find_first_of('/');
    for(;;){

        it_s = rule.find_first_not_of('/', it_s);
        if(it_s == string::npos){
            return res.size() > 0;
        }

        sRule tmp_rule;
        tmp_rule.rand = false;
        tmp_rule.name.clear();

        if(rule[it_s] == ':'){
            tmp_rule.rand = true;
            it_s++;
        }

        auto it_e = rule.find_first_of(":/", it_s);
        if(rule[it_e] == ':'){
            res.clear();
            return false;
        }

        tmp_rule.name = rule.substr(it_s, (it_e - it_s));
        if(tmp_rule.name.length() < 1){
            res.clear();
            return false;
        }

        res.push_back(tmp_rule);
        it_s = it_e;


    }
}

string MyRouter::get_rule_by_id(const int id){

    if(id < 0 || size_t(id) >= rules.size()){
        return "";
    }

    string res = "/";

    for(auto x:rules[id]){
        if(x.rand){
            res += ":";
        }
        res += x.name;
        res += "/";
    }

    return res;
}

int MyRouter::last_added(){

    if(last_added_rule == rules.end()){
        return NOT_FOUND;
    }

    return std::distance(rules.begin(), last_added_rule);
}

int MyRouter::add_rule(const string rule){

    std::vector<sRule> tmp;
    tmp.clear();
    if(check_rule(rule, tmp)){
        rules.push_back(tmp);
        last_added_rule = std::prev(rules.end());
    }

    return last_added();
}


int MyRouter::find_request(const string req,
                           unordered_map<string, string> *param){

    std::vector<sRule> tmp;
    if(!check_rule(req, tmp)){
        return NOT_FOUND;
    }

    if(param != nullptr){
        param->clear();
    }

    for (auto it = rules.begin(); it != rules.end(); ++it){

        if(it->size() != tmp.size()){
            continue;
        }

        bool match = true;

        for(size_t i = 0; i < it->size(); i++){
            if(tmp[i].rand){
                tmp[i].name = ":" + tmp[i].name;
                tmp[i].rand = false;
            }

            if((*it)[i].rand){
                if(param != nullptr){
                    param->insert({(*it)[i].name, tmp[i].name});
                }
                continue;
            }

            if((*it)[i].name.compare(tmp[i].name) != 0){
                match = false;
                break;
            }
        }

        if(match){
            return std::distance(rules.begin(), it);
        }else{
            if(param != nullptr){
                param->clear();
            }
        }


    }

    return NOT_FOUND;
}
