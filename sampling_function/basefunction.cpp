#include <assert.h>
#include "basefunction.h"

BaseFunction::BaseFunction()
{
    //something to do
}


void BaseFunctionTests(){

    BaseFunction test;

    for (int i= 0; i < 100; i++){
        assert(test.Evaluate(i) == i);
    }

    assert(test.Evaluate(0.00001) != 1);

}
