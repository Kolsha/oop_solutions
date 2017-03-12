#include <istream>
#include <math.h>
#include <map>
#include "sampler.h"

using namespace std;


Sampler::Sampler(BaseFunction *func_to_samp)
{
    set_func(func_to_samp);
}



map<double, double> *Sampler::sample(double from, double to, double step){


    if(fabs(step) < than_zero || func == nullptr){
        return nullptr;
    }

    if(from == to){
        return nullptr;
    }

    map<double, double> *res = new map<double, double>;
    if(res == nullptr){
        return nullptr;
    }

    try
    {
        double x = from;
        double y = 0;
        while(x <= to){
            y = func->Evaluate(x);
            res->insert(pair<double,double>(x, y));
            x += step;
        }

    }catch (...)
    {
        return nullptr;
    }

    return res;

}

bool Sampler::set_func(BaseFunction *func_to_samp){

    if(func_to_samp == nullptr){
        return false;
    }
    func = func_to_samp;

    return true;
}

