#include <iostream>
#include "my_router.h"
using namespace std;

int main()
{
    MyRouter router;
    string tmp = "";
    while (tmp != "exit"){
        cin >> tmp;
        int id = router.add_rule(tmp);
        cout << id << endl;
        cout << router.get_rule_by_id(id) << endl;
    }

    cout << router.find_request("/test/test") << endl;

    return 0;
}

