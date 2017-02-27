#ifndef BASEFUNCTION_H
#define BASEFUNCTION_H


class BaseFunction
{
public:
    BaseFunction();
    virtual double Evaluate(double x){
        return x;
    }
};

void BaseFunctionTests();

#endif // BASEFUNCTION_H
