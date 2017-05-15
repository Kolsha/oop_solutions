#ifndef STOCKMARKET_H
#define STOCKMARKET_H

#include <unordered_map>
#include <vector>

struct RateInfo{
    double value;
    time_t ts;
};


class StockMarket
{
public:
    static StockMarket& Instance()
    {
        // согласно стандарту, этот код ленивый и потокобезопасный
        static StockMarket stock;
        return stock;
    }
    bool new_rate(const std::string rate, const double value);
    std::vector< RateInfo >& get_rates(const std::string rate);
private:
    std::unordered_map< std::string, std::vector< RateInfo > > rates;
    StockMarket() {}
    ~StockMarket() {}

    StockMarket(StockMarket const&) = delete;
    StockMarket& operator= (StockMarket const&) = delete;
};

#endif // STOCKMARKET_H
