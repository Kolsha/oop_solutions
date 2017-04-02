#include "gtest/gtest.h"
#include "ini_tests.h"
#include "read_conf.h"

using namespace std;

TEST(SuccessWay, FirstTest) {
    string stringvalues = "name foo\n" "foo 200 # 400\n"
                          "'foo_bar' 400 # 200\n" "#bar 400\n"
                          "  spaces '     '  # здесь 5 пробелов\n"
                          "full_name \" Vasya Pupkin \" "
                          "# пробелы в начале и конце строки"
                          "double_quote   '\"' \n"
                          "single_quote \"'\" \n"
                          "\nboth_quotes2 \"'\\\"\" \n"
                          "\n\n\n\n\n\n";

    istringstream iss(stringvalues);


    map<string, string> cfg = readConfiguration(iss, nullptr);

    ASSERT_EQ( cfg.at("spaces"), "     ");

    ASSERT_EQ( cfg.at("full_name"), " Vasya Pupkin ");

    ASSERT_EQ( cfg.at("name"), "foo");

    ASSERT_EQ( cfg.at("foo"), "200");

    ASSERT_EQ( cfg.at("single_quote"), "'");
}

