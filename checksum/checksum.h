#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <unordered_map>
#include <vector>

class CheckSum
{
private:
    std::unordered_map< std::string, std::string > files;
    std::string calc_checksum(const std::string &filename);
public:
    CheckSum() = default ;
    CheckSum(const std::vector< std::string > &files);
    CheckSum(const std::string  &filename);
    void add_files(const std::vector< std::string > &files);
    void add_file(const std::string &filename);
    void clear(){
        files.clear();
    }

    const std::unordered_map< std::string, std::string > &get_checksums(){
        return files;
    }
    std::string get_checksum(const std::string &filename);
};

#endif // CHECKSUM_H
