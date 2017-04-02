#ifndef MYROUTER_H
#define MYROUTER_H

#include <vector>
#include <string>
#include <map>

using namespace std;

class MyRouter
{
protected:
    vector <string> rules;
    bool check_rule(const string rule);
public:

    inline void clear(){
        rules.clear();
    }

    int add_rule(const string rule);
    string get_rule_by_id(const int id);
    int find_request(const string req,
                     const map<string, string> *param);
    MyRouter();
};

#endif // MYROUTER_H
