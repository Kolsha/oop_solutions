#ifndef SQRTFUNC_H
#define SQRTFUNC_H

#include "basefunction.h"

class SqrtFunc : public BaseFunction
{
public:
    SqrtFunc();
    double Evaluate(double x) override;
};

void SqrtFuncTests();

#endif // SQRTFUNC_H
