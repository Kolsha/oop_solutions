#ifndef MYROUTER_H
#define MYROUTER_H

#include <vector>
#include <string>
#include <unordered_map>

#define NOT_FOUND -1

typedef struct tsRule {
    std::string name;
    bool rand;
} sRule;


class MyRouter
{
protected:
    std::vector <std::vector<sRule>> rules;
    std::vector <std::vector<sRule>>::iterator last_added_rule;
    bool check_rule(const std::string rule, std::vector<sRule> &res);
public:
    inline void clear(){
        rules.clear();
    }

    int last_added();

    int add_rule(const std::string rule);

    std::string get_rule_by_id(const int id);
    int find_request(const std::string req,
                     std::unordered_map<std::string, std::string> *param = nullptr);

    MyRouter();
    MyRouter(const std::string rule);
};

#endif // MYROUTER_H
