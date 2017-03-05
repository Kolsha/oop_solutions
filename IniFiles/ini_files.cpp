#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>

#include "ini_files.h"

using namespace std;

IniFiles::IniFiles()
{

}

IniFiles::IniFiles(std::string file_name)
{
    readConfiguration(file_name);
}

IniFiles::IniFiles(std::istream &is)
{
    readConfiguration(is);
}


IniFiles::~IniFiles()
{
    params.clear();
}


bool IniFiles::readConfiguration(std::string file_name){

    last_error = "";

    try{
        std::ifstream ifs(file_name.c_str());

        if(ifs.is_open()){
            bool result = readConfiguration(ifs);
            ifs.close();
            return result;
        }
        last_error = "Fail in open file: " + file_name;
    }
    catch(std::exception e){
        last_error = e.what();
    }

    return false;
}

void IniFiles::skip_whitespace(std::string str, size_t &pos){

    if(pos >= str.length()){
        return ;
    }

    while(str[pos] == ' '){
        pos++;
    }
}

bool IniFiles::readParam(std::string line){

    if(line.length() < MIN_PARAM_LINE_LENGTH){
        return false;
    }

    size_t pos = 0;
    std::string key = "", value = "";

    skip_whitespace(line, pos);

}

bool IniFiles::readConfiguration(std::istream &is){

    params.clear();
    size_t readed = 0;
    std::string line;
    while(std::getline(is, line)){

        cout << line << endl;
        if(readParam(line)){
            readed++;
        }
    }

    return (readed > 0);

}



void IniFilesTests(){

    std::string stringvalues = "Test   test\n2   3";
    std::istringstream iss(stringvalues);

    IniFiles *ini = new IniFiles(iss);

    delete ini;
}


