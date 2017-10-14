#include <stdio.h>
#include "cache.h"

namespace Kolsha {

bool FileCache::file_exists(const std::string &fn)
{
    std::ifstream f(make_unique_name(fn));
    return f.good();
}

std::string FileCache::make_unique_name(const std::string &name, size_t first_letter){
    std::string res = "fc_";
    size_t hs = std::hash<std::string>()(name);


    size_t fs_count = 0;
    for(auto &c : name){
        if(fs_count < first_letter){
            if(isalnum(c)){
                res += c;
                fs_count++;
            }
        }
    }
    res += "_" + std::to_string(hs);
    return std::move(res);
}

std::string FileCache::read_from_file(const std::string &fn)
{
    std::ifstream stream(make_unique_name(fn));
    if(!stream.is_open()){
        return std::string();
    }
    std::string str((std::istreambuf_iterator<char>(stream)),
                    std::istreambuf_iterator<char>());
    stream.close();
    return str;
}

void FileCache::write_to_file(const std::string &fn, const std::string &value)
{
    std::ofstream stream(make_unique_name(fn));
    if(!stream.is_open()){
        return ;
    }
    stream << value;
    stream.close();
}

void FileCache::remove_file(const std::string &fn)
{
    if(file_exists(fn)){
        std::string unic_fn = make_unique_name(fn);
        std::remove(unic_fn.c_str());
    }
}

}
