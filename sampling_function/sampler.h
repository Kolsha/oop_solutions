#ifndef SAMPLER_H
#define SAMPLER_H

#include <iostream>
#include "basefunction.h"

class Sampler
{
protected:
    BaseFunction *func;
    const double than_zero = 0.00000000001;
public:
    Sampler(BaseFunction *func_to_samp);
    bool change_func(BaseFunction *func_to_samp);
    bool start_sample(double from, double to, double step);
};

#endif // SAMPLER_H
