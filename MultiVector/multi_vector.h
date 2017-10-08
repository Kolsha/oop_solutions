#pragma once
#include <iostream>
#include <array>

#define VECTOR_DUMP(x) std::cout << #x << " " << x << std::endl

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


    typename std::array<Key_T, dim>::iterator begin(){
        return coors.begin();
    }
    typename std::array<Key_T, dim>::const_iterator begin() const{
        return coors.begin();
    }

    typename std::array<Key_T, dim>::iterator end(){
        return coors.end();
    }

    typename std::array<Key_T, dim>::const_iterator end() const{
        return coors.end();
    }

    void fill(const Key_T& val){
        for (size_t idx = 0; idx < dim; idx++){
            coors[idx] = val;
        }
    }
    void swap(multi_vector& other){
        for (size_t idx = 0; idx < dim; idx++){
            std::swap(coors[idx], other[idx]);
        }
    }

    multi_vector& operator+=(const multi_vector& rhs)
    {
        for (size_t idx = 0; idx < dim; idx++){
            coors[idx] += rhs[idx];
        }
        return *this;
    }

    multi_vector operator+(const multi_vector& rhs){
        multi_vector tmp;
        for(size_t idx = 0; idx < dim; idx++){
            tmp[idx] = coors[idx] + rhs[idx];
        }

        return std::move(tmp);
    }

    template<typename T>
    multi_vector& operator*=(const T& val){

        for(size_t idx = 0; idx < dim; idx++){
            coors[idx] *= val;
        }
        return *this;
    }

    template<typename T>
    multi_vector operator*(const T& val) {
        multi_vector tmp;
        for(size_t idx = 0; idx < dim; idx++){
            tmp[idx] = coors[idx] * val;
        }
        return std::move(tmp);
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



    multi_vector& operator*=(const multi_vector& rhs){

        static_assert(dim == 3, "Dimension can be only 3");
        std::array<Key_T, dim> lhs = coors;

        const size_t x = 0, y = 1, z = 2;

        /*
         * i(aybz - azby) - j(axbz - azbx) + k(axby - aybx)
         */

        coors[x] = (lhs[y] * rhs[z] - lhs[z] * rhs[y]);

        coors[y] = (lhs[x] * rhs[z] - lhs[z] * rhs[x]) * (-1);

        coors[z] = (lhs[x] * rhs[y] - lhs[y] * rhs[x]);

        return *this;
    }

    multi_vector operator*(const multi_vector& rhs){
        static_assert(dim == 3, "Dimension can be only 3");
        std::array<Key_T, dim> lhs = coors;
        multi_vector tmp;

        const size_t x = 0, y = 1, z = 2;

        /*
         * i(aybz - azby) - j(axbz - azbx) + k(axby - aybx)
         */

        tmp[x] = (lhs[y] * rhs[z] - lhs[z] * rhs[y]);

        tmp[y] = (lhs[x] * rhs[z] - lhs[z] * rhs[x]) * (-1);

        tmp[z] = (lhs[x] * rhs[y] - lhs[y] * rhs[x]);

        return std::move(tmp);
    }

    bool operator==(const multi_vector& rhs){
        for (size_t idx = 0; idx < dim; idx++){
            if(coors[idx] != rhs[idx]){
                return false;
            }
        }
        return true;
    }

    bool operator!=(const multi_vector& rhs){
        return !((*this) == rhs);
    }

    bool operator< (const multi_vector& rhs){
        for (size_t idx = 0; idx < dim; idx++){
            if(coors[idx] >= rhs[idx]){
                return false;
            }
        }
        return true;
    }

};






template <typename Key_T,
          size_t dim = 3>
std::ostream& operator<<(std::ostream& out, const multi_vector<Key_T, dim>& vec)
{
    out << "{";
    for(size_t idx = 0; idx < dim; idx++){
        out << vec[idx];
        if((idx + 1) < dim){
            out << ", ";
        }
    }
    return out << "}";
}

/*
 *
 * http://en.cppreference.com/w/cpp/language/operators
 */
