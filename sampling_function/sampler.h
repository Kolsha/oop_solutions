#ifndef SAMPLER_H
#define SAMPLER_H

#include <iostream>
#include <fstream>




#include "basefunction.h"



class Sampler
{
protected:
    BaseFunction *func;
    std::string file_name;
    const double than_zero = 0.00000000001;
public:
    Sampler(BaseFunction *func_to_samp);
    Sampler(BaseFunction *func_to_samp, std::string FN);
    bool set_file_name(std::string FN);
    bool set_func(BaseFunction *func_to_samp);
    bool sample(double from, double to, double step);
};

#endif // SAMPLER_H
