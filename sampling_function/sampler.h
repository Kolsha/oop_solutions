#ifndef SAMPLER_H
#define SAMPLER_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>


#include "basefunction.h"


using namespace std;

typedef  double (*func_to_sample)(double);

class Sampler
{
protected:
    BaseFunction *func;
    func_to_sample func_ptr;
    const double than_zero = 0.00000000001;
    void clear(){
        func = nullptr;
        func_ptr = nullptr;
    }

public:
    Sampler(BaseFunction *func_to_samp);
    Sampler(func_to_sample func_to_samp);
    bool set_func(BaseFunction *func_to_samp);
    bool set_func(func_to_sample func_to_samp);
    vector<double> sample(double from, double to, double step);
};


#endif // SAMPLER_H
