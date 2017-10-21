#pragma once

#include <iostream>

namespace {


constexpr unsigned int salts[] = {
    0x1953c322, 0x588ccf17, 0x64bf600c, 0xa6be3f3d,
    0x341a02ea, 0x15b03217, 0x3b062858, 0x5956fd06,
    0x18b5624f, 0xe3be0b46, 0x20ffcd5c, 0xa35dfd2b,
    0x1fc4a9bf, 0x57c45d5c, 0xa8661c4a, 0x4f1b74d2,
    0x5a6dde13, 0x3b18dac6, 0x05a8afbf, 0xbbda2fe2,
    0xa2520d78, 0xe7934849, 0xd541bc75, 0x09a55b57,
    0x9b345ae2, 0xfc2d26af, 0x38679cef, 0x81bd1e0d,
    0x654681ae, 0x4b3d87ad, 0xd5ff10fb, 0x23b32f67,
    0xafc7e366, 0xdd955ead, 0xe7c34b1c, 0xfeace0a6,
    0xeb16f09d, 0x3c57a72d, 0x2c8294c5, 0xba92662a,
    0xcd5b2d14, 0x743936c8, 0x2489beff, 0xc6c56e00,
    0x74a4f606, 0xb244a94a, 0x5edfc423, 0xf1901934,
    0x24af7691, 0xf6c98b25, 0xea25af46, 0x76d5f2e6,
    0x5e33cdf2, 0x445eb357, 0x88556bd2, 0x70d1da7a,
    0x54449368, 0x381020bc, 0x1c0520bf, 0xf7e44942,
    0xa27e2a58, 0x66866fc5, 0x12519ce7, 0x437a8456,
};

constexpr size_t salts_sz = sizeof(salts) / sizeof(unsigned int);


}

template<
        class Val,
        size_t  num_functions = salts_sz,
        size_t table_size = 128,
        class Hash = std::hash<Val>,
        class Allocator = std::allocator<unsigned char>
        >
class BloomFilter
{
private:
    Allocator alc;
    Hash hash_func;
    size_t table_sz = (table_size + 7) / 8;
    unsigned char *table = nullptr;
public:
    BloomFilter(){
        static_assert((num_functions <= salts_sz), "To many functions");
        table = alc.allocate(table_sz);
    }

    BloomFilter(const BloomFilter& other){
        std::copy(other.table, other.table + table_sz, table);
    }

    BloomFilter(BloomFilter&& other){
        table = other.table;
        other.table = nullptr;
    }

    BloomFilter &operator=(const BloomFilter& other){
        if(this != &other){
            std::copy(other.table, other.table + table_sz, table);
        }
        return *this;
    }

    BloomFilter &operator=(BloomFilter&& other){
        if(this != &other){
            table = other.table;
            other.table = nullptr;
        }
        return *this;
    }

    void insert(const Val& value){

        size_t hash = hash_func(value);
        for (size_t i = 0; i < num_functions; ++i) {

            /* Generate a unique hash */

            size_t subhash = hash ^ salts[i];

            /* Find the index into the table */

            size_t index = subhash % table_size;

            /* Insert into the table.
             * index / 8 finds the byte index of the table,
             * index % 8 gives the bit index within that byte to set. */

            unsigned char b = (unsigned char) (1 << (index % 8));
            table[index / 8] |= b;
        }
    }

    bool query(const Val& value){
        size_t hash = hash_func(value);
        for (size_t i = 0; i < num_functions; ++i) {

            /* Generate a unique hash */

            size_t subhash = hash ^ salts[i];

            /* Find the index into the table to test */

            size_t index = subhash % table_size;

            /* The byte at index / 8 holds the value to test */

            unsigned char b = table[index / 8];
            int bit = 1 << (index % 8);

            /* Test if the particular bit is set; if it is not set,
             * this value can not have been inserted. */

            if ((b & bit) == 0) {
                return false;
            }
        }

        /* All necessary bits were set.  This may indicate that the value
         * was inserted, or the values could have been set through other
         * insertions. */

        return true;
    }

    BloomFilter operator+(const BloomFilter& other) {
        BloomFilter tmp;
        for (size_t i = 0; i < table_sz; ++i) {
            tmp.table[i] = table[i] | other.table[i];
        }
        return std::move(tmp);
    }

    BloomFilter operator*(const BloomFilter& other) {
        BloomFilter tmp;
        for (size_t i = 0; i < table_sz; ++i) {
            tmp.table[i] = table[i] & other.table[i];
        }
        return std::move(tmp);
    }

    virtual ~BloomFilter(){
        if(table != nullptr)
            alc.deallocate(table, table_sz);
    }


};
