#include <iostream>
#include <sstream>
#include <assert.h>

#include "ini_tests.h"
#include "read_conf.h"

using namespace std;

void IniFilesTests(){

    string stringvalues = "";

    stringvalues.append("     name           foo\n");

    stringvalues.append("foo 200 # 400\n");
    stringvalues.append("'foo_bar' 400 # 200\n");

    stringvalues.append("#bar 400\n");

    stringvalues.append("'bad_param_name' \"val\\#ue\" dsf \n");

    stringvalues.append("  spaces '     '  # здесь 5 пробелов\n");

    stringvalues.append("name \" Vasya Pupkin \"   # пробелы в начале и конце строки");


    stringvalues.append("test                 case adf #sdfsdf\n");


    stringvalues.append("double_quote   '\"' \n");

    stringvalues.append("single_quote \"'\" \n");

    stringvalues.append("\nboth_quotes2 \"'\\\"\" \n");

    stringvalues.append("\n\n\n\n\n\n");


    stringvalues.append("both_quotes1 1\\\\\"test-sss#213");

    istringstream iss(stringvalues);

    string errors;

    map<string, string> cfg = readConfiguration(iss, &errors);

    for (auto& kv : cfg) {
        cout << kv.first << " = " << kv.second << endl;
    }

    assert( cfg.at("spaces") == string("     "));

    assert( cfg.at("name") == string(" Vasya Pupkin "));

    assert( cfg.at("both_quotes1") == string("1\\\"test-sss"));

    assert(!cfg.at("test").compare("case adf"));
    cout << endl << errors << endl;
}

