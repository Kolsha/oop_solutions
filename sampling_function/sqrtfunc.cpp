#include <math.h>
#include <assert.h>
#include "sqrtfunc.h"

SqrtFunc::SqrtFunc()
{

}

double SqrtFunc::Evaluate(double x){

    return sqrt(x);
}


void SqrtFuncTests(){
    SqrtFunc test;

    double x, y;

    //***************//
    x = 4;
    y = sqrt(x);
    assert(test.Evaluate(x) == y);
    //***************//

    //***************//
    x = 88;
    y = sqrt(x);
    assert(test.Evaluate(x) == y);
    //***************//

    //***************//
    for(int i = 1; i < 100; i++){
        x = 88;
        y = sqrt(x);
        assert(test.Evaluate(x) == y);
    }
    //***************//

    assert(test.Evaluate(11) != 11);
}
