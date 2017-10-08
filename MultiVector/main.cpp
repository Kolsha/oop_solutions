#include <iostream>
#include "multi_vector.h"


using namespace std;

int main(int argc, char *argv[])
{
    multi_vector<double, 2> a({1, 1}), c({1, 2});
    multi_vector<double, 2> d = (a + c) * 3.33;

    VECTOR_DUMP(a);
    VECTOR_DUMP(d);
    a.swap(d);
    VECTOR_DUMP(a);
    VECTOR_DUMP(d);

   /* multi_vector<double, 3> m = a * c;

    VECTOR_DUMP(m);

    auto tmp = (a * c);
    cout << (m == tmp) << endl;

    for(auto &c:d){
        cout << c << endl;
    }
    */

    cout << "Hello World!" << endl;
    return 0;
}
