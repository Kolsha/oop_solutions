#pragma once

#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <typeinfo>
#include <cstring>
#include "serialize_exception.h"
using std::endl;
using std::cout;



namespace{

namespace SerBytes {
static const uint8_t DELIMETR = 0x11;

static const uint8_t RAW = 0x01;
static const uint8_t STR = 0x02;
static const uint8_t VEC = 0x03;
static const uint8_t MAP = 0x01;
static const uint8_t UNDEF = 0xFF;
}




void write_byte(std::ostream &os, uint8_t oct = SerBytes::DELIMETR){
    os.write((char*) &oct, sizeof(uint8_t));
}

bool read_before_byte(std::istream &is, uint8_t oct = SerBytes::DELIMETR)
{

    uint8_t tmp = oct + 1;
    while(tmp != oct && is){
        is >> tmp;
    }

    return (tmp == oct);
}

uint8_t read_ser_type(std::istream &is){
    uint8_t oct = SerBytes::UNDEF;
    if(!read_before_byte(is, SerBytes::DELIMETR)){
        return oct;
    }

    is >> oct;
    return oct;
}




}





///////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////


/*
 * SERIALIZE *
 */
template <typename T>
void serialize(const T &obj, std::ostream &os)
{
    write_byte(os, SerBytes::DELIMETR);
    write_byte(os, SerBytes::RAW);

    size_t len;
    if(typeid(T).name() == typeid(char*).name()){
        len = std::strlen((char*)obj) * sizeof(char);
        os.write((char*)obj, len);
    }else{
        len = sizeof(T);
        const uint8_t *ptr = reinterpret_cast<const uint8_t *>(&obj);
        std::ostream_iterator<uint8_t> oi(os);
        std::copy(ptr, ptr + len, oi);
    }


    write_byte(os, SerBytes::RAW);
    write_byte(os, SerBytes::DELIMETR);
}


template <typename T>
void serialize(const std::vector<T> &obj, std::ostream &os)
{
    cout << "Ser: vector" << endl;
    write_byte(os, SerBytes::DELIMETR);
    write_byte(os, SerBytes::VEC);
    for (auto &row : obj) // access by reference to avoid copying
    {
        serialize(row, os);
    }
    write_byte(os, SerBytes::VEC);
    write_byte(os, SerBytes::DELIMETR);
}

template<>
void serialize<std::string>(const std::string &obj, std::ostream &os)
{
    cout << "Ser: string" << endl;
    write_byte(os, SerBytes::DELIMETR);
    write_byte(os, SerBytes::STR);
    serialize((char*)obj.c_str(), os);
    write_byte(os, SerBytes::STR);
    write_byte(os, SerBytes::DELIMETR);
}








template <typename K, typename V>
void serialize(const std::map<K, V> &obj, std::ostream &os)
{
    cout << "Ser: map" << endl;
    write_byte(os, SerBytes::DELIMETR);
    write_byte(os, SerBytes::MAP);
    for (auto& kv : obj)
    {
        serialize(kv.first, os);
        serialize(kv.second, os);
    }
    write_byte(os, SerBytes::MAP);
    write_byte(os, SerBytes::DELIMETR);
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
    uint8_t oct = read_ser_type(is);
    if(oct != SerBytes::RAW){
        throw RAWDeserializationException();
    }


    uint8_t *ptr = reinterpret_cast<uint8_t*>(&obj);
    std::istream_iterator<uint8_t> ii(is);

    std::copy_n(ii, sizeof(T), ptr);

    read_before_byte(is, SerBytes::DELIMETR);
}


template<>
void deserialize<char*>(char* &obj, std::istream &is)
{
    cout << "DeSer: raw" << endl;
    uint8_t oct = read_ser_type(is);
    if(oct != SerBytes::RAW){
        throw RAWDeserializationException();
    }

    size_t start_pos = is.tellg();
    if(!read_before_byte(is, SerBytes::RAW)){
        throw RAWDeserializationException();
    }

    size_t len = (is.tellg()  - start_pos);

    cout << len << endl;

    char *tmp = new char[len];
    is.seekg(start_pos);
    is.read(tmp, len - 1);
    tmp[len - 1] = '\0';
    obj = std::move((char*)tmp);
    read_before_byte(is, SerBytes::DELIMETR);
}



template <typename T>
void deserialize(std::vector<T> &obj, std::istream &is)
{
    cout << "DeSer: vector" << endl;
    uint8_t oct = read_ser_type(is);
    if(oct != SerBytes::VEC){
        throw VectorDeserializationException();
    }
    while (is.peek() == SerBytes::DELIMETR)
    {
        T tmp;
        deserialize(tmp, is);
        obj.push_back(tmp);
    }
    read_before_byte(is, SerBytes::DELIMETR);
}

template<>
void deserialize<std::string>(std::string &obj, std::istream &is)
{
    cout << "DeSer: string" << endl;
    uint8_t oct = read_ser_type(is);
    if(oct != SerBytes::STR){
        throw StringDeserializationException();
    }
    char *tmp;
    deserialize(tmp, is);
    obj.clear();
    obj.assign(tmp);
    read_before_byte(is, SerBytes::DELIMETR);
}




template <typename K, typename V>
void deserialize(std::map<K, V> &obj, std::istream &is)
{
    cout << "DeSer: map" << endl;
    uint8_t oct = read_ser_type(is);
    if(oct != SerBytes::MAP){
        throw MapDeserializationException();
    }
    while (is.peek() == SerBytes::DELIMETR)
    {
        std::pair<K, V> temp_pair;
        deserialize(temp_pair.first, is);
        deserialize(temp_pair.second, is);
        obj.insert(temp_pair);
    }
    read_before_byte(is, SerBytes::DELIMETR);
}


/*
 * DESERIALIZE *
 */
