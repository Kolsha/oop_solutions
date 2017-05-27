#ifndef SPENDROW_H
#define SPENDROW_H
#include <unordered_map>
#include <map>

class SpendRow final
{
private:
    static const char cat_del = ':';
    std::unordered_map< std::string, SpendRow>* child = nullptr;//std::unique_ptr
    std::map<time_t, double> buys;
public:

    SpendRow(){}
    SpendRow(const std::string& cat, const time_t ts, const double sum);
    bool insert_buy(const std::string &cat, const time_t ts,  const double sum);
    double get_sum(const std::string& cat, const time_t start = 0,
                   const time_t end = 0);

    ~SpendRow(){
        if(child){
            delete child;
        }
    }

    SpendRow(SpendRow&& tmp)
        :   child(tmp.child)
        ,   buys(tmp.buys)
    {
        tmp.child = nullptr;
    }


    SpendRow(const SpendRow& other)
        : buys(other.buys)
    {
        if(other.child){
            child = new std::unordered_map<std::string, SpendRow>;
            child->insert(other.child->begin(), other.child->end());
        }
    }


};

#endif // SPENDROW_H
