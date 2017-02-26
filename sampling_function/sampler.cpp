#include <istream>
#include <math.h>
#include "sampler.h"

using namespace std;


Sampler::Sampler(BaseFunction *func_to_samp)
{
    func = func_to_samp;
}


bool Sampler::start_sample(double from, double to, double step){

    double tmp;

    if(fabs(step) < than_zero || func == nullptr){
        return false;
    }

    if(from > to){
        tmp = from;
        from = to;
        to = tmp;
    }else if(from == to){
        return false;
    }

    double indx = from;
    std::string x_log = "*x* ";
    std::string y_log = "*y* ";
    while(indx <= to){

        try
        {
            tmp = func->Evaluate(indx);

            x_log.append(to_string(indx));
            x_log.append(" | ");

            y_log.append(to_string(tmp));
            y_log.append(" | ");
        }
        catch (...)
        {
            return false;
        }


        indx += step;
    }

    std::cout << x_log << endl;
    std::cout << y_log << endl;

    return true;

}

bool Sampler::change_func(BaseFunction *func_to_samp){

    if(func_to_samp == nullptr){
        return false;
    }
    func = func_to_samp;

    return true;
}
