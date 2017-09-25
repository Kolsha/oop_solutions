#include <iostream>
#include "merging_iterator.h"

class Comparator
{
public:
    int operator () (int a, int b)
    {
        return a > b ? 1: 0;
    }
    int operator () (double a, double b)
    {
        return a > b ? 1: 0;
    }

};


using namespace std;

int main(int argc, char *argv[])
{
    Comparator my_comp;
    vector<int> int_arr = {1, 2, 7, 3, 5, 8, 6, 10, 12};

    vector<vector<int>::iterator> first = {
        int_arr.begin(),
        int_arr.begin() + 4,
        int_arr.begin() + 8


    };
    vector<vector<int>::iterator>        last ={
        int_arr.begin() + 3,
        int_arr.begin() + 7,
        int_arr.end()

    };
    merge_sort(first, last, my_comp);
    for (size_t i = 0; i < int_arr.size(); ++i)
    {
        if (int_arr[i] != int(i))
        {
            cout << "fai;" << endl;
        }
    }
}
