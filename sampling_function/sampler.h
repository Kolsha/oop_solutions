#ifndef SAMPLER_H
#define SAMPLER_H

#include <iostream>
#include <fstream>
#include <map>



#include "basefunction.h"


using namespace std;

class Sampler
{
protected:
    BaseFunction *func;

    const double than_zero = 0.00000000001;
public:
    Sampler(BaseFunction *func_to_samp);
    bool set_func(BaseFunction *func_to_samp);
    map<double, double> *sample(double from, double to, double step);
};


#endif // SAMPLER_H
