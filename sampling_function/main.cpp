#include <iostream>
#include "basefunction.h"
#include "sampler.h"
#include "sqrtfunc.h"

using namespace std;

int main()
{
    cout << "Started!" << endl;
    BaseFunctionTests();
    SqrtFuncTests();
    try{
        double from, to, step;
        std::ifstream ifs("in.txt");
        ifs >> from >> to >> step;
        ifs.close();

        SqrtFunc func;

        Sampler smp(&func, "output.txt");

        if(smp.sample(from, to, step)){
            cout << "Success" << endl;
        }else{
            cout << "Fail" << endl;
        }

    }
    catch(...){
        return 1;
    }

    return 0;
}

