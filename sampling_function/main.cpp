#include <iostream>
#include "basefunction.h"
#include "sampler.h"
#include "sqrtfunc.h"

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    SqrtFunc func;

    cout << func.Evaluate(10) << endl;

    Sampler smp(&func);

    smp.start_sample(0, 0.05, 0.005);
    return 0;
}

