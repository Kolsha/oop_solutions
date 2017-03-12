#ifndef READ_CONF_H
#define READ_CONF_H
#include <iostream>
#include <map>

#define WHITE_SPACES " \t"
#define COMMENT_STR "#"

using namespace std;

map<string, string> readConfiguration(istream &is,
                                      string *perrors = nullptr);

#endif // READ_CONF_H
