#include <iostream>
#include <sstream>
#include <assert.h>

#include "ini_files.h"
#include "ini_tests.h"


using namespace std;

void IniFilesTests(){
    string stringvalues = "";

    stringvalues.append("name \" Vasya ' ' \\\" Pupkin \"   # 1");
    stringvalues.append("\n      spaces ' \\\\ @\"'  # здесь 5 пробелов");



    stringvalues.append("\ntest                 case adf #sdfsdf");


    stringvalues.append("\ndouble_quote   '\"'");

    stringvalues.append("\nsingle_quote \"'\"");

    stringvalues.append("\nboth_quotes2 \"'\\\"\"");

    stringvalues.append("\nboth_quotes1 1\\\\\"test-sss#213");

    istringstream iss(stringvalues);

    IniFiles *ini = new IniFiles(iss);

    for (auto& kv : ini->getParamsMap()) {
        cout << kv.first << " = " << kv.second << endl;
    }

    assert( ini->getParamsMap().size() == 6 );

    assert( ini->getParamsMap().at("both_quotes1").compare("1\\\"test-sss"));

    assert(!ini->getParamsMap().at("test").compare("case adf"));

    delete ini;

}

