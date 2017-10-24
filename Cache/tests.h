#pragma once

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"
#include "cache.h"


using namespace Kolsha;
using namespace std;


TEST(CacheTest, First) {

    NullCache null;
    PoorManMemoryCache poor(1);
    CacheApplier cache(null);

    string str = "12312312";

    cache.write_to_cache("test", str);
    ASSERT_FALSE(cache.has_cache("test"));
    ASSERT_EQ(cache.read_from_cache("test"), "");

    cache.set_strategy(poor);

    cache.write_to_cache("test", str);
    ASSERT_TRUE(cache.has_cache("test"));
    ASSERT_EQ(cache.read_from_cache("test"), str);

    cache.write_to_cache("test1", str);
    ASSERT_FALSE(cache.has_cache("test"));

}


TEST(CacheTest, Second) {

    FileCache file;
    MemoryCache mem;
    CacheApplier cache(file);

    string str = "sdfsdfsdfsd";

    cache.write_to_cache("test", str);
    ASSERT_TRUE(cache.has_cache("test"));
    ASSERT_EQ(cache.read_from_cache("test"), str);

    cache.set_strategy(mem);

    cache.write_to_cache("test", str);
    ASSERT_TRUE(cache.has_cache("test"));
    ASSERT_EQ(cache.read_from_cache("test"), str);

    cache.write_to_cache("test1", str);
    ASSERT_TRUE(cache.has_cache("test"));
    ASSERT_TRUE(cache.has_cache("test1"));

}


TEST(CacheTest, templateTest) {


    CacheApplier_T<MemoryCache> cache;

    string str = "sdfsdfsdfsd";

    cache.write_to_cache("test", str);
    ASSERT_TRUE(cache.has_cache("test"));
    ASSERT_EQ(cache.read_from_cache("test"), str);

}



TEST(CacheTest, PoorManTest) {

    const size_t poor_sz = 10;
    PoorManMemoryCache poor(poor_sz);
    CacheApplier cache(poor);

    string str = "12312312";
    for(size_t i = 0; i < poor_sz; i++){
        cache.write_to_cache("test" + to_string(i), str);
    }

    for(size_t i = 0; i < poor_sz; i++){
        ASSERT_TRUE(cache.has_cache("test" + to_string(i)));
    }

    for(size_t i = 0; i < poor_sz; i++){
        cache.write_to_cache("2test" + to_string(i), str);
    }

    for(size_t i = 0; i < poor_sz; i++){
        ASSERT_FALSE(cache.has_cache("test" + to_string(i)));

    }


}




