#ifndef INIFILES_H
#define INIFILES_H

#include <iostream>
#include <map>

#define MIN_PARAM_LINE_LENGTH 3
#define WHITE_SPACES " \t"
#define COMMENT_STR "#"

using namespace std;

class IniFiles
{
private:
    char is_quote(char c) const;
    bool prepare_value(string &val);
    string trim(const string& str,
                const string& whitespace = WHITE_SPACES) const;
protected:
    map<string, string> params;
    string last_error;
    bool readParam(string line);
    void setError(string err){
        last_error = err;
        cout << err << endl;
    }

public:
    IniFiles();
    IniFiles(const string file_name);
    IniFiles(istream &is);
    ~IniFiles();

    bool readConfiguration(istream &is);

    bool readConfiguration(const string file_name);

    string getLastError(){
        return last_error;
    }

    void clear(){
        params.clear();
    }

    const map<string, string> getParamsMap(){
        return params;
    }
};



#endif // INIFILES_H
