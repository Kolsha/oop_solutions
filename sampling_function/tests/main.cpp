#include <iostream>

#include "gtest/gtest.h"
#include "../basefunction.h"
#include "../sampler.h"
using namespace std;


TEST(SamplerTest, BaseFunctionTest) {
    BaseFunction func;

    Sampler smp(&func);

    vector<double> res = smp.sample(0, 10, 1);

    ASSERT_TRUE(res.size() == 11);

    for(int i = 0; i <= 10; i++){
        ASSERT_EQ(res[i], i);
    }

}

double my_func(double x){
    return (x - 1) * (x + 1);
}

TEST(SamplerTest, FuncPtrTest) {

    Sampler smp(&my_func);

    vector<double> res = smp.sample(0, 10, 1);

    ASSERT_TRUE(res.size() == 11);

    for(int i = 0; i <= 10; i++){
        ASSERT_EQ(res[i], my_func(i));
    }

}



int main(int argc, char ** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

