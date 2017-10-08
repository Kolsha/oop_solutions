#include <iostream>
#include "multi_vector.h"
using namespace std;

int main(int argc, char *argv[])
{
    multi_vector<double, 3> a({1, 1, 1}), c({1, 2, 3});
    multi_vector<double, 3> d = (a + c) * 3.33;


    cout << "Hello World!" << endl;
    return 0;
}
