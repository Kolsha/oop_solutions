#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>
#include "checksum.h"


CheckSum::CheckSum(const std::vector< std::string > &files){
    add_files(files);
}

CheckSum::CheckSum(const std::string  &filename){
    add_file(filename);
}

void CheckSum::add_files(const std::vector< std::string > &files){
    for (auto& fn : files) {
        add_file(fn);
    }
}

void CheckSum::add_file(const std::string &filename){
    if(files.find(filename) == files.end()){
        std::string checksum = calc_checksum(filename);
        if(!checksum.empty()){
            files.insert({filename, checksum});
        }
    }
}

std::string CheckSum::get_checksum(const std::string &filename){

    auto it = files.find(filename);
    if(it == files.end()){
        return "";
    }

    return std::string(it->second);
}


std::string CheckSum::calc_checksum(const std::string &filename){

    std::ifstream fp(filename);

    try{

        std::stringstream ss;

        if (!fp.is_open()) {
            return "";
        }

        uint32_t magic = 5381;
        char c;
        while (fp.get(c)) {
            magic = ((magic << 5) + magic) + c;
        }
        fp.close();

        ss << std::hex << std::setw(8) << std::setfill('0') << magic;
        return ss.str();
    }
    catch(...){
        fp.close();
        return "";
    }
}
