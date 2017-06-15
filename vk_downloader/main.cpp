#include <iostream>
#include <ctime>
#include "app_logic.h"

using namespace std;


int main(int argc, char *argv[])
{
    if(argc < 2 || argc > 4){
        cout << "[access_token] login pass" << endl;
        return 1;
    }

    string access_token, login, pass;

    if(argc == 2){
        access_token = argv[1];
    }
    else if(argc == 3){
        login = argv[1];
        pass = argv[2];
    }
    else if(argc == 4){
        access_token = argv[1];
        login = argv[2];
        pass = argv[3];
    }

    AppLogic app;

    clock_t begin = clock();
    int res = app.run(login, pass, access_token);
    app.wait_for_all_downloaded();
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    cout << endl << "Works: " << elapsed_secs << " sec." << endl;
    return res;
}
