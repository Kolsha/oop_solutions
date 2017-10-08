#pragma once
#include <iostream>
#include <array>
#include <initializer_list>

template <typename Key_T,
          size_t dim = 3>
class multi_vector
{
private:
    std::array<Key_T, dim> coors;
public:

    multi_vector(){}
    multi_vector(const std::array<Key_T, dim> init){
        for (size_t idx = 0; idx < dim; idx++){
            coors[idx] = init[idx];
        }
    }

    ~multi_vector(){}

    multi_vector& operator+=(const multi_vector& rhs)
    {
        for (size_t idx = 0; idx < dim; idx++){
            coors[idx] += rhs[idx];
        }
        return *this;
    }

    multi_vector operator+(const multi_vector& rhs) {
        /*multi_vector tmp;
        for(size_t idx = 0; idx < dim; idx++){
            tmp[idx] = coors[idx] + rhs[idx];
        }
        return std::move(tmp);*/
        return (*this) += rhs;
    }

    template<typename T>
    multi_vector& operator*=(const T& val)
    {
        for(size_t idx = 0; idx < dim; idx++){
            coors[idx] *= val;
        }
        return *this;
    }

    template<typename T>
    multi_vector operator*(const T& val) {
        return (*this) *= val;
    }

    Key_T& operator[](size_t idx){
        if (idx < dim){
            return coors[idx];
        }
        throw std::out_of_range("index more than dimension");
    }

    const Key_T& operator[](size_t idx) const{
        if (idx < dim){
            return coors[idx];
        }
        throw std::out_of_range("index more than dimension");
    }





    Key_T& scalar_multiply(const multi_vector& rhs){
        Key_T tmp(0);
        for (size_t idx = 0; idx < dim; idx++){
            tmp += coors[idx] * rhs[idx];
        }
        return std::move(tmp);
    }



    multi_vector& operator*=(const multi_vector& rhs)
    {

        return *this;
    }


};
