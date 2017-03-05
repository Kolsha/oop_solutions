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
    clear();
}


bool IniFiles::readConfiguration(std::string file_name){

    setError("");

    try{
        std::ifstream ifs(file_name.c_str());

        if(ifs.is_open()){
            bool result = readConfiguration(ifs);
            ifs.close();
            return result;
        }
        setError("Fail in open file: " + file_name);
    }
    catch(std::exception e){
        setError(e.what());
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

char IniFiles::is_quote(char c){

    switch(c){
    case '\'':
        return '\'';
    case '"':
        return '"';
    }

    return 0;
}

std::string trim(const std::string& str,
                 const std::string& whitespace = WHITE_SPACES)
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
    {
        return ""; // no content
    }

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool IniFiles::readParam(std::string line){

    line = trim(line);
    if(line.length() < MIN_PARAM_LINE_LENGTH){
        return false;
    }

    std::string key = "", value = "";

    //cout <<"|"   << line <<"|"<< endl;

    const auto key_end = line.find(WHITE_SPACES[0]);

    if (key_end == std::string::npos)
    {
        setError("Empty key");
        return false;
    }

    key = line.substr(0, key_end);

    /* may be optimised */
    value = line.substr(key_end);
    value = trim(value);
    /* may be optimised */



    const auto key_has_comment = key.find(COMMENT_STR);

    if (key_has_comment != std::string::npos)
    {
        setError("Comment in key");
        return false;
    }

    const auto value_has_comment = value.find(COMMENT_STR);

    value = value.substr(0, value_has_comment);

    if(!value.length()){
        setError("Empty value");
        return false;
    }

    /* check value */

    /* check value */

    cout << key << " = " << value << endl;

    try{
        params.insert(pair<std::string,std::string>(key, value));
        return true;
    }
    catch(std::exception e){
        setError(e.what());
    }

    return false;

}

bool IniFiles::readConfiguration(std::istream &is){

    clear();
    size_t readed = 0;
    std::string line;
    while(std::getline(is, line)){

        //cout << line << endl;
        try{
            if(readParam(line)){
                readed++;
            }
        }
        catch(std::exception e){
            setError(e.what());
        }

    }

    return (readed > 0);

}



void IniFilesTests(){

    std::string stringvalues = "        Test        test\n              2    3  \n        ";
    std::istringstream iss(stringvalues);

    IniFiles *ini = new IniFiles(iss);

    delete ini;
}


