#include <iostream>
#include "basefunction.h"
#include "sampler.h"
#include "sqrtfunc.h"
#include "sampler_tests.h"

using namespace std;

int main()
{
    cout << "Started!" << endl;
    BaseFunctionTests();
    SqrtFuncTests();
    SamplerTests();
    try{
        double from, to, step;
        ifstream ifs("in.txt");
        ifs >> from >> to >> step;
        ifs.close();

        SqrtFunc func;

        Sampler smp(&func);

        map<double, double> *res = smp.sample(from, to, step);
        if(res != nullptr){
            try{
                ofstream ofs("out.txt");
                ofs << "x y" << endl;
                for (const auto& key_val : *res) {
                    ofs << key_val.first << " " << key_val.second << endl;
                }
                ofs.close();
                cout << "Success" << endl;
            }
            catch(...){
                cout << "Fail" << endl;
                return 1;
            }
            delete res;
        }else{
            cout << "Fail" << endl;
        }


    }
    catch(...){
        return 1;
    }

    return 0;
}

