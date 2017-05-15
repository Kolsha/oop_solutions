#include <iostream>
#include "stockmarket.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout << "Hello World!" << endl;
    StockMarket& stock = StockMarket::Instance();

    return 0;
}
