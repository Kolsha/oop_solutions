#include <fstream>
#include <iostream>
#include "gtest/gtest.h"
#include "tests.h"
#include "serialize.h"

using std::ofstream;
using std::ifstream;

TEST(Serialize, RawTest) {

    int a = rand();
    int a_real = a;

    double b = rand() * 0.11;
    double b_real = b;
    char *c = "new test suk",
            *c_real = c;

    ofstream ofs("RAW_TEST.ser", ofstream::out | ofstream::binary);

    serialize(a, ofs);
    serialize(b, ofs);
    serialize(c, ofs);
    ofs.close();

    a = rand();
    b = rand();
    c = NULL;

    ifstream ifs("RAW_TEST.ser", ifstream::in | ofstream::binary);
    ifs >> std::noskipws;

    deserialize(a, ifs);
    deserialize(b, ifs);
    deserialize(c, ifs);
    ifs.close();

    ASSERT_EQ(a, a_real);
    ASSERT_EQ(b, b_real);
    ASSERT_EQ(strcmp(c_real, c), 0);

}

TEST(Serialize, StrTest) {

    std::string str = "str ytests mm",
            str_real = str;
    ofstream ofs("Str_TEST.ser", ofstream::out | ofstream::binary);

    serialize(str, ofs);
    ofs.close();

    str.clear();

    ifstream ifs("Str_TEST.ser", ifstream::in | ofstream::binary);
    ifs >> std::noskipws;

    deserialize(str, ifs);
    ifs.close();

    ASSERT_TRUE(str == str_real);
}


TEST(Serialize, MapTest) {

    std::map<size_t, int> a, a_real;
    for(size_t i = 1; i <= size_t(rand() + 10); i++){
        int tmp = rand();
        a.insert({i, tmp});
        a_real.insert({i, tmp});
    }
    ofstream ofs("MapTest.ser", ofstream::out | ofstream::binary);

    serialize(a, ofs);
    ofs.close();

    a.clear();

    ifstream ifs("MapTest.ser", ifstream::in | ofstream::binary);
    ifs >> std::noskipws;

    deserialize(a, ifs);
    ifs.close();

    ASSERT_EQ(a.size(), a_real.size());
    for(auto &kv : a){
        ASSERT_EQ(a_real[kv.first], kv.second);
    }
}


TEST(Serialize, VecTEST) {

    std::vector<int> a, a_real;
    for(size_t i = 1; i <= size_t(rand() + 10); i++){
        int tmp = rand();
        a.push_back(tmp);
        a_real.push_back(tmp);
    }
    ofstream ofs("VecTEST.ser", ofstream::out | ofstream::binary);

    serialize(a, ofs);
    ofs.close();

    a.clear();

    ifstream ifs("VecTEST.ser", ifstream::in | ofstream::binary);
    ifs >> std::noskipws;

    deserialize(a, ifs);
    ifs.close();

    ASSERT_EQ(a.size(), a_real.size());
    for(size_t i = 0; i < a.size(); i++){
        ASSERT_EQ(a[i], a_real[i]);
    }
}

TEST(Serialize, BadFileTEST) {
    int a = 0;
    ifstream ifs("VecTEST.ser", ifstream::in | ofstream::binary);
    ifs >> std::noskipws;

    try{
        deserialize(a, ifs);
        SUCCEED();
    }
    catch(RAWDeserializationException &exp){

    }
    catch(...){
        FAIL();
    }

    ifs.close();
    FAIL();

}



int run_tests(int argc, char *argv[]){
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
