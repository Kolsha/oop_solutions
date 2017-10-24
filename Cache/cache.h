#pragma once

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <vector>
#include <algorithm>

namespace Kolsha {





class BaseCache {
public:
    virtual bool has(const std::string &key) = 0;
    virtual std::string read(const std::string &key) = 0;
    virtual void write(const std::string &key, const std::string &value) = 0;
    virtual void remove(const std::string &key) = 0;
    virtual ~BaseCache() {}
};




template <typename T>
class CacheApplier_T {
private:
    T strat;
public:
    bool has_cache(const std::string &key){
        return strat.has(key);
    }

    std::string read_from_cache(const std::string &key){
        return strat.read(key);
    }

    void write_to_cache(const std::string &key, const std::string &value){
        strat.write(key, value);
    }

    void remove_from_cache(const std::string &key){
        strat.remove(key);
    }
};



class CacheApplier {
private:
    BaseCache* strat;
public:
    CacheApplier(BaseCache& strategy): strat(&strategy){}
    inline void set_strategy(BaseCache& strategy){
        strat = &strategy;
    }

    bool has_cache(const std::string &key){
        return strat->has(key);
    }

    std::string read_from_cache(const std::string &key){
        return strat->read(key);
    }

    void write_to_cache(const std::string &key, const std::string &value){
        strat->write(key, value);
    }

    void remove_from_cache(const std::string &key){
        strat->remove(key);
    }
};







class MemoryCache: public BaseCache{
protected:
    std::unordered_map<std::string, std::string> storage;
public:
    inline bool has(const std::string &key){
        return storage.find(key) != storage.end();
    }

    inline std::string read(const std::string &key){
        return storage.at(key);
    }

    inline void write(const std::string &key, const std::string &value){
        storage[key] = value;
    }
    inline void remove(const std::string &key){
        storage.erase(key);
    }
    virtual ~MemoryCache() {}

};

class NullCache: public BaseCache{
public:
    inline bool has(const std::string &){
        return false;
    }

    inline std::string read(const std::string &){
        return std::string("");
    }

    inline void write(const std::string &, const std::string &){
        return ;
    }
    inline void remove(const std::string &){
        return ;
    }
    virtual ~NullCache() {}
};

class PoorManMemoryCache: public BaseCache{
private:
    size_t sz;
protected:
    using val = std::pair<std::string, std::string>;
    std::vector<val> storage;
    std::vector<val>::iterator find_by_key(const std::string &key){
        auto it = std::find_if( storage.begin(), storage.end(),
                                [&key](const val& p){ return p.first == key;} );
        return it;
    }

public:
    PoorManMemoryCache(size_t _size = 32) :
        sz(_size){}

    inline size_t size(size_t new_size = 0){
        if(new_size > 0){
            sz = new_size;
        }
        return sz;
    }

    inline bool has(const std::string &key){
        return std::any_of(storage.begin(), storage.end(),
                           [&key](const val& p)
        { return p.first == key; });
    }

    inline std::string read(const std::string &key){
        auto it = find_by_key(key);
        if(it == storage.end()){
            throw std::out_of_range("Key not found");
        }
        return it->second;
    }

    inline void write(const std::string &key, const std::string &value){
        if(storage.size() >= sz){
            storage.erase(storage.begin());
        }
        auto it = find_by_key(key);
        val pair = {key, value};
        if(it != storage.end()){
            storage.erase(it);
        }
        storage.push_back(pair);
    }
    inline void remove(const std::string &key){
        auto it = find_by_key(key);
        if(it != storage.end()){
            storage.erase(it);
        }
    }
    virtual ~PoorManMemoryCache() {}

};


class FileCache: public BaseCache{
private:
    bool file_exists(const std::string& fn);
    std::string make_unique_name(const std::string& name, size_t first_letter = 5);
    std::string read_from_file(const std::string& fn);
    void write_to_file(const std::string& fn, const std::string &value);
    void remove_file(const std::string& fn);

public:
    FileCache(){

    }

    inline bool has(const std::string &key){
        return file_exists(key);
    }

    inline std::string read(const std::string &key){
        return read_from_file(key);
    }

    inline void write(const std::string &key, const std::string &value){
        write_to_file(key, value);
    }
    inline void remove(const std::string &key){
        remove_file(key);
    }
    virtual ~FileCache() {}
};

}

