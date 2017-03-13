#include <iostream>
#include "basefunction.h"
#include "sampler.h"
#include "sqrtfunc.h"
#include "sampler_tests.h"

using namespace std;

double my_func(double x){
    return x*x;
}

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

        vector<double> res = smp.sample(from, to, step);

        try{
            ofstream ofs("out.txt");
            ofs << "*x* | ";
            while(from <= to){
                ofs << from << " | ";
                from += step;
            }
            ofs << endl << "*y* | ";
            for (const auto& val : res) {
                ofs << val << " | ";
            }
            ofs.close();
            cout << "Success" << endl;
        }
        catch(...){
            cout << "Fail" << endl;
            return 1;
        }

    }
    catch(...){
        return 1;
    }

    return 0;
}

