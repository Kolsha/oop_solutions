#include "gtest/gtest.h"
#include "ini_tests.h"
#include "read_conf.h"

using namespace std;

TEST(SuccessWay, FirstTest) {
    string stringvalues = "";

    stringvalues.append("name foo\n");

    stringvalues.append("foo 200 # 400\n");
    stringvalues.append("'foo_bar' 400 # 200\n");

    stringvalues.append("#bar 400\n");
    stringvalues.append("  spaces '     '  # здесь 5 пробелов\n");

    stringvalues.append(
                "full_name \" Vasya Pupkin \"  # пробелы в начале и конце строки");

    stringvalues.append("double_quote   '\"' \n");

    stringvalues.append("single_quote \"'\" \n");

    stringvalues.append("\nboth_quotes2 \"'\\\"\" \n");

    stringvalues.append("\n\n\n\n\n\n");

    istringstream iss(stringvalues);


    map<string, string> cfg = readConfiguration(iss, nullptr);

    ASSERT_EQ( cfg.at("spaces"), "     ");

    ASSERT_EQ( cfg.at("full_name"), " Vasya Pupkin ");

    ASSERT_EQ( cfg.at("name"), "foo");

    ASSERT_EQ( cfg.at("foo"), "200");

    ASSERT_EQ( cfg.at("single_quote"), "'");
}

