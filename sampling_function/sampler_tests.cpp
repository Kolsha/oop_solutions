#include <assert.h>
#include "basefunction.h"
#include "sampler.h"

#include "sampler_tests.h"

void SamplerTests(){
    BaseFunction func;
    Sampler smp(&func);
    map<double, double> *res = smp.sample(0, 10, 1);
    assert(res != NULL);
    assert(res->size() == 11);
    double t = 0;
    for (const auto& key_val : *res) {
        assert(t == key_val.first);
        assert(t == key_val.second);
        t++;
    }
    delete res;

    smp.set_func(nullptr);
    res = smp.sample(0, 0, -1);

    assert(res == nullptr);
}
