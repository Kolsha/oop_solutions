#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

#include <assert.h>

#include "ini_files.h"

using namespace std;

IniFiles::IniFiles()
{

}

IniFiles::IniFiles(string file_name)
{
    readConfiguration(file_name);
}

IniFiles::IniFiles(istream &is)
{
    readConfiguration(is);
}

IniFiles::~IniFiles()
{
    clear();
}

bool IniFiles::readConfiguration(const string file_name){

    setError("");

    try{
        ifstream ifs(file_name.c_str());

        if(ifs.is_open()){
            bool result = readConfiguration(ifs);
            ifs.close();
            return result;
        }
        setError("Fail in open file: " + file_name);
    }
    catch(exception e){
        setError(e.what());
    }

    return false;
}

char IniFiles::is_quote(char c) const{

    switch(c){
    case '\'':
        return '\'';
    case '"':
        return '"';
    }

    return 0;
}

string IniFiles::trim(const string& str,
                      const string& whitespace) const
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == string::npos)
    {
        return ""; // no content
    }

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

bool IniFiles::prepare_value(string &val){

    if(!val.length()){
        return false;
    }

    bool q_open = false;
    char prev = '\0', main_q = '\0';

    string res = "";

    for (size_t pos = 0; pos < val.length(); pos++)
    {

        char cur = val[pos];

        if(prev == '\\'){
            switch(cur){
            case '\\':
                res.push_back(cur);
                prev = '\0';
                break;
            case 'n':
            case 'r':
                prev = '\n'; //res.append("\n");
                break;
            case 't':
                prev = '\t'; //res.append("\t");
                break;
            default:
                if(!main_q || cur != main_q){
                    setError("Escape error");
                    return false;
                }
                prev = main_q; //res.append(main_q);

            }

            continue;
        }

        if(cur == main_q){
            q_open = !q_open;
            continue;
        }

        if(pos < 1 && is_quote(cur)){
            main_q = cur;
            //second_q = (main_q == '\'') ? '"' : '\'';
            q_open = true;
            continue;
        }

        if(pos > 0 && cur == ' ' && !q_open){
            setError("Whitespace in wrong place");
            return false;
        }

        res.push_back(prev);
        prev = cur;
    }

    res.push_back(prev);
    val.assign(res);

    return true;
}

bool IniFiles::readParam(string line){

    line = trim(line);

    if(line.length() < MIN_PARAM_LINE_LENGTH){
        return false;
    }

    string key = "", value = "";

    const auto key_end = line.find(WHITE_SPACES[0]);

    if (key_end == string::npos)
    {
        setError("Empty key");
        return false;
    }

    key = line.substr(0, key_end);

    const auto key_has_comment = key.find(COMMENT_STR);

    if (key_has_comment != string::npos)
    {
        setError("Comment in key");
        return false;
    }

    value = line.substr(key_end);

    const auto value_has_comment = value.find(COMMENT_STR);

    value = value.substr(0, value_has_comment);
    value = trim(value);


    if(!value.length()){
        setError("Empty value");
        return false;
    }

    if(!prepare_value(value)){
        setError("Bad value");
        return false;
    }

    try{
        params.insert(pair<string,string>(key, value));
        return true;
    }
    catch(exception e){
        setError(e.what());
    }

    return false;

}

bool IniFiles::readConfiguration(istream &is){

    clear();
    size_t readed = 0;
    string line;
    while(getline(is, line)){

        try{
            if(readParam(line)){
                readed++;
            }
        }
        catch(exception e){
            setError(e.what());
        }

    }

    return (readed > 0);

}



