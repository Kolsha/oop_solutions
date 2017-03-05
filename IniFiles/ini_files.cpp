#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

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


bool IniFiles::readConfiguration(const std::string file_name){

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

char IniFiles::is_quote(char c){

    switch(c){
    case '\'':
        return '\'';
    case '"':
        return '"';
    }

    return 0;
}

std::string IniFiles::trim(const std::string& str,
                           const std::string& whitespace)
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

string string_replace(const string & s, const string & findS, const std::string & replaceS )
{
    string result = s;
    auto pos = s.find( findS );
    if ( pos == string::npos ) {
        return result;
    }
    result.replace( pos, findS.length(), replaceS );
    return string_replace( result, findS, replaceS );
}

bool IniFiles::prepare_value(string &val){

    static vector< pair< string, string > > patterns = {
        { "\\\\" , "\\" },
        { "\\n", "\n" },
        { "\\r", "\r" },
        { "\\t", "\t" },
        { "\\\"", "\"" },
        { "\\'", "'" }
    };

    //val = string_replace(val, patterns[0].first, patterns[0].second);


    cout << val << endl;

    if(!val.length()){
        false;
    }

    /* check value */
    size_t count_bs = 0, pos = 0;
    bool q1 = false, q2 = false, result = true;
    char prev = '\0', tmp = '\0', main_q = '\0';

    for (char cur : val)
    {

        if(pos < 1 && is_quote(cur)){
            main_q = cur;
        }
        else if(is_quote(cur) && !main_q &&
                (prev != '\\' || count_bs % 2 == 0)){

            setError("Quote in wrong place");
            return false;
        }

        pos++;

        if(!(q1 || q2) && cur == WHITE_SPACES[0]){

            setError("Whitespace in wrong place");
            return false;
        }

        switch(cur){
        case '\\':
            count_bs++;
            break;
        case '\'':{

            if(count_bs % 2 == 0){
                q1 = !q1;
            }
            break;
        }

        case '"':{

            if(count_bs % 2 == 0){
                q2 = !q2;
            }
            break;
        }
        default:
            cout << "";
        }

        prev = cur;
    }

    //return result;
    /* check value

    for (const auto & p : patterns){
        val = string_replace(val, p.first, p.second);
    }
*/

}

bool IniFiles::readParam(std::string line){

    line = trim(line);
    if(line.length() < MIN_PARAM_LINE_LENGTH){
        return false;
    }

    std::string key = "", value = "";

    cout <<"|"   << line <<"|"<< endl;

    const auto key_end = line.find(WHITE_SPACES[0]);

    if (key_end == std::string::npos)
    {
        setError("Empty key");
        return false;
    }

    key = line.substr(0, key_end);

    const auto key_has_comment = key.find(COMMENT_STR);

    if (key_has_comment != std::string::npos)
    {
        setError("Comment in key");
        return false;
    }

    /* may be optimised */
    value = line.substr(key_end);

    const auto value_has_comment = value.find(COMMENT_STR);

    value = value.substr(0, value_has_comment);
    value = trim(value);
    /* may be optimised */




    if(!value.length()){
        setError("Empty value");
        return false;
    }

    if(!prepare_value(value)){
        setError("Empty value");
        return false;
    }



    //cout << key << " = " << value << endl;

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

    std::string stringvalues =
            "name \" Vasya ' ' \\\" Pupkin \"   # пробелы в начале и конце строки";
    stringvalues.append("\n      spaces ' \\\\ \\    '  # здесь 5 пробелов");

    stringvalues.append("\ntest                 case adf #sdfsdf");


    stringvalues.append("\ndouble_quote   '\"'");

    stringvalues.append("\nsingle_quote \"'\"");

    stringvalues.append("\nboth_quotes \"'\\\"\"");

    stringvalues.append("\nboth_quotes 1\\\\\"test-sss");
    std::istringstream iss(stringvalues);

    IniFiles *ini = new IniFiles(iss);

    delete ini;
}


