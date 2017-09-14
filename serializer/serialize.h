#pragma once

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <string>

using std::endl;
using std::cout;

namespace{

static const uint8_t MAGIC_DELIMETR = 0x11;

static const uint8_t SER_TYPE_RAW = 0x01;
static const uint8_t SER_TYPE_STR = 0x02;
static const uint8_t SER_TYPE_VEC = 0x03;
static const uint8_t SER_TYPE_MAP = 0x01;

void write_byte(std::ostream &os, uint8_t oct = MAGIC_DELIMETR){
    os.write((char*) &oct, sizeof(uint8_t));
}

}

template <typename T, typename K = T>
struct serializer {
private:
    static void apply_for_raw_obj(const T &obj, std::ostream &os) {


        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&obj);
        std::ostream_iterator<uint8_t> oi(os);

        std::copy(ptr, ptr + sizeof(T), oi);


    }
public:

    static void apply(const T &obj, std::ostream &os) {
        os << 'r' << '{';
        apply_for_raw_obj(obj, os);
        os << '}';
    }


    static void apply(const std::vector<T> &obj, std::ostream &os) {
        std::cout << "ser vector" << endl;
        for (auto &row : obj) // access by reference to avoid copying
        {
            std::cout << row.first << endl;
        }
    }

    static void apply(const std::map<K, T> &obj, std::ostream &os) {
        std::cout << "ser map" << endl;
    }

    /*static void apply(const std::string &obj, std::ostream &os) {
        std::cout << "ser string" << endl;
    }
    */


    /*
    */






};






///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////


/*
 * SERIALIZE *
 */


template <typename T>
void serialize(const T &obj, std::ostream &os)
{
    cout << "Ser: raw" << endl;
    write_byte(os, MAGIC_DELIMETR);
    write_byte(os, SER_TYPE_RAW);
    const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&obj);
    std::ostream_iterator<uint8_t> oi(os);
    std::copy(ptr, ptr + sizeof(T), oi);
    write_byte(os, MAGIC_DELIMETR);
}

template <typename T>
void serialize(const std::vector<T> &obj, std::ostream &os)
{
    cout << "Ser: vector" << endl;
    for (auto &row : obj) // access by reference to avoid copying
    {
        //std::cout << row. << endl;
    }
}

template<>
void serialize<std::string>(const std::string &obj, std::ostream &os)
{
    cout << "Ser: string" << endl;
}



template <typename K, typename V>
void serialize(const std::map<K, V> &obj, std::ostream &os)
{
    cout << "Ser: map" << endl;
}


/*
 * SERIALIZE *
 */


/*
 * DESERIALIZE *
 */


template <typename T>
void deserialize(T &obj, std::istream &is)
{
    cout << "DeSer: raw" << endl;
    uint8_t oct = 0;
    while(oct != MAGIC_DELIMETR && is){
        is >> oct;
    }

    if(oct != MAGIC_DELIMETR){
        cout << "can't find magic" << endl;
        return ;
        //throw ;
    }

    is >> oct;
    if(oct != SER_TYPE_RAW){
        cout << "Bad file" << endl;
        return ;
    }
    uint8_t *ptr = reinterpret_cast<uint8_t*>(&obj);
    std::istream_iterator<uint8_t> ii(is);

    std::copy_n(ii, sizeof(T), ptr);
}





template <typename T>
void deserialize(std::vector<T> &obj, std::istream &is)
{
    cout << "DeSer: vector" << endl;
    for (auto &row : obj) // access by reference to avoid copying
    {
        //std::cout << row. << endl;
    }
}

template<>
void deserialize<std::string>(std::string &obj, std::istream &is)
{
    cout << "DeSer: string" << endl;
}



template <typename K, typename V>
void deserialize(std::map<K, V> &obj, std::istream &is)
{
    cout << "DeSer: map" << endl;
}


/*
 * DESERIALIZE *
 */
