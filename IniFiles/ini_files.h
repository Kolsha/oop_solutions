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
    void skip_whitespace(std::string str, size_t &pos);
    char is_quote(char c);
protected:
    std::map<std::string, std::string> params;
    std::string last_error;
    bool readParam(std::string line);
    void setError(std::string err){
        last_error = err;
    }

public:
    IniFiles();
    IniFiles(std::string file_name);
    IniFiles(std::istream &is);
    ~IniFiles();

    bool readConfiguration(std::istream &is);

    bool readConfiguration(std::string file_name);

    std::string getLastError(){
        return last_error;
    }

    void clear(){
        params.clear();
    }
};

void IniFilesTests();
#endif // INIFILES_H
