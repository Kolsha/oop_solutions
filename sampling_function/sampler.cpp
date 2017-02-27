#include <istream>
#include <math.h>
#include "sampler.h"

using namespace std;


Sampler::Sampler(BaseFunction *func_to_samp):
    func(func_to_samp)
{
    //func = func_to_samp;
}

Sampler::Sampler(BaseFunction *func_to_samp, std::string FN):
    func(func_to_samp), file_name(FN)
{

    //func = func_to_samp;

    /*if(FN.length() > 0){
        file_name = FN;
    }*/
}

bool Sampler::set_file_name(std::string FN){

    if(FN.length() > 0){
        file_name = FN;
        return true;
    }

    return true;
}


bool Sampler::sample(double from, double to, double step){

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

    try
    {
        double indx = from;
        std::string x_log = "*x* ";
        std::string y_log = "*y* ";

        while(indx <= to){


            tmp = func->Evaluate(indx);

            x_log.append(to_string(indx));
            x_log.append(" | ");

            y_log.append(to_string(tmp));
            y_log.append(" | ");




            indx += step;
        }

        if(file_name.length() > 0){
            ofstream log(file_name);
            log << x_log << endl;
            log << y_log << endl;
            log.close();
        }else{
            std::cout << x_log << endl;
            std::cout << y_log << endl;
        }

    }catch (...)
    {
        return false;
    }

    return true;

}

bool Sampler::set_func(BaseFunction *func_to_samp){

    if(func_to_samp == nullptr){
        return false;
    }
    func = func_to_samp;

    return true;
}
