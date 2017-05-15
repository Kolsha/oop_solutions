#ifndef SPENDROW_H
#define SPENDROW_H
#include <unordered_map>
#include <map>
class SpendRow
{
private:
    static const char cat_del = ':';
    std::unordered_map< std::string, SpendRow>* child = nullptr;
    std::map<time_t, double> buys;
public:

    SpendRow(){}
    SpendRow(const std::string cat, const time_t ts, const double sum);
    bool insert_buy(const std::string cat, const time_t ts,  const double sum);
    double get_sum(const std::string cat, const time_t start = 0,
                   const time_t end = 0);

};

#endif // SPENDROW_H
