#include <istream>
#include <math.h>
#include <map>
#include "sampler.h"

using namespace std;


Sampler::Sampler(BaseFunction *func_to_samp)
{
    set_func(func_to_samp);
}

Sampler::Sampler(func_to_sample func_to_samp)
{
    set_func(func_to_samp);
}


vector<double> Sampler::sample(double from, double to, double step){

    vector<double> res;
    if(fabs(step) < than_zero ||
            (func == nullptr && func_ptr == nullptr)){
        return res;
    }

    if(from == to){
        return res;
    }




    try
    {
        double x = from;
        double y = 0;
        while(x <= to){
            if(func != nullptr){
                y = func->Evaluate(x);
            }else if(func_ptr != nullptr){
                y = func_ptr(x);
            }else{
                return res;
            }

            res.push_back(y);
            x += step;
        }

    }catch (...)
    {
        return res;
    }

    return res;

}

bool Sampler::set_func(BaseFunction *func_to_samp){

    if(func_to_samp == nullptr){
        return false;
    }
    clear();
    func = func_to_samp;

    return true;
}

bool Sampler::set_func(func_to_sample func_to_samp){

    if(func_to_samp == nullptr){
        return false;
    }
    clear();
    func_ptr = func_to_samp;

    return true;
}

