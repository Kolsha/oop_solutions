#pragma once

#define _USE_MATH_DEFINES

#include <iostream>
#include <sstream>

#include "gtest/gtest.h"
#include "bloom_filter.hpp"


using namespace std;

TEST(bloom_filter, insert_query)
{
    BloomFilter<string, 4> filter;


    ASSERT_FALSE(filter.query("test 1"));
    ASSERT_FALSE(filter.query("test 2"));

    /* Insert some values */

    filter.insert("test 1");
    filter.insert("test 2");

    /* Check they are set */

    ASSERT_TRUE(filter.query("test 1"));
    ASSERT_TRUE(filter.query("test 2"));
}

TEST(bloom_filter,read_load)
{
    BloomFilter<string, 4> filter1, filter2;
    /* Create a filter with some values set */

    filter1.insert("test 1");
    filter1.insert("test 2");
    /* Read the current state into an array */

    filter2 = filter1;
    /* Check the values are set in the new filter */

    ASSERT_TRUE(filter2.query("test 1"));
    ASSERT_TRUE(filter2.query("test 2"));
}

TEST(bloom_filter,intersection)
{
    BloomFilter<string, 4> filter1, filter2, result;

    /* Create one filter with both values set */

    filter1.insert("test 1");
    filter1.insert("test 2");

    /* Create second filter with only one value set */

    filter2.insert("test 1");

    /* For this test, we need this to be definitely not present.
     * Note that this could theoretically return non-zero here,
     * depending on the hash function. */

    ASSERT_FALSE(filter2.query("test 2"));

    /* Intersection */

    result = filter1 * filter2;

    /* test 1 should be set, as it is in both
     * test 2 should not be set, as it is not present in both */
    ASSERT_TRUE(result.query("test 1"));
    ASSERT_FALSE(result.query("test 2"));
}

TEST(bloom_filter, union_test)
{
    BloomFilter<string, 4> filter1, filter2, result;

    /* Create one filter with both values set */

    filter1.insert("test 1");

    /* Create second filter with only one value set */

    filter2.insert("test 2");

    /* Union */

    result = filter1 + filter2;

    /* Both should be present */

    ASSERT_TRUE(result.query("test 1"));
    ASSERT_TRUE(result.query("test 2"));
}



TEST(bloom_filter, another_type)
{
    BloomFilter<double, 4> filter1, filter2, result;


    /* Create one filter with both values set */

    filter1.insert(M_PI);

    /* Create second filter with only one value set */

    filter2.insert(M_E);

    /* Union */

    result = filter1 + filter2;

    /* Both should be present */

    ASSERT_TRUE(result.query(M_PI));
    ASSERT_TRUE(result.query(M_E));
}






