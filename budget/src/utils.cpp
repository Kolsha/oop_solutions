#include "utils.h"

using namespace std;

void trim(string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return ;
    }
    size_t last = str.find_last_not_of(' ');
    str = str.substr(first, (last - first + 1));
}

bool explode(const string& str, const char& ch,
             vector<string>& result, bool need_trim) {

    string next;
    if(str.empty()){
        return false;
    }

    result.clear();

    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        if (*it == ch) {
            if(need_trim)
                trim(next);
            if (!next.empty()) {

                result.push_back(next);
                next.clear();
            }
        } else {
            next += *it;
        }
    }

    if(need_trim)
        trim(next);

    if (!next.empty()){
        result.push_back(next);
    }

    return (result.size() > 0);
}
