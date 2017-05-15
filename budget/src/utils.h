#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

void trim(std::string& str);
bool explode(const std::string& str, const char& ch,
             std::vector<std::string> &result, bool need_trim = false);

#endif // UTILS_H
