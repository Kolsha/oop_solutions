#ifndef READ_CONF_H
#define READ_CONF_H
#include <iostream>
#include <map>

#define WHITE_SPACES " \t"
#define COMMENT_STR "#"

/* read config file. e.g.:
 * 'foo' bar    #comment
 * bar 'foo'    #olol
 * #ololololololololol
 */
std::map<std::string, std::string> readConfiguration(std::istream &is,
                                      std::string *perrors = nullptr);

#endif // READ_CONF_H
