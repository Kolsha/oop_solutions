#pragma once
#include <iostream>
#include <array>

#define VECTOR_DUMP(x) std::cout << #x << " " << x << std::endl

template <typename T, size_t dim = 3>
using Matrix = std::array<std::array<T, dim>, dim>;



template <typename Key_T,
          size_t dim = 3>
class multi_vector
{
private:
    std::array<Key_T, dim> coors;
    multi_vector<Key_T, dim + 1> expandToTramsform() const {
        multi_vector<Key_T, dim + 1> result;
        std::copy(begin(), end(), result.begin());
        result[dim] = 1;
        return result;
    }
    void vector_multiply(multi_vector &res, const multi_vector& lhs,  const multi_vector& rhs){
        static_assert(dim == 3, "Dimension can be only 3");
        const size_t x = 0, y = 1, z = 2;

        /*
         * i(aybz - azby) - j(axbz - azbx) + k(axby - aybx)
         */

        res[x] = (lhs[y] * rhs[z] - lhs[z] * rhs[y]);

        res[y] = (lhs[x] * rhs[z] - lhs[z] * rhs[x]) * (-1);

        res[z] = (lhs[x] * rhs[y] - lhs[y] * rhs[x]);

    }

public:

    multi_vector(){
        coors.fill(0);
    }
    multi_vector(const std::array<Key_T, dim>& init){
        std::copy(init.begin(), init.end(), begin());
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
        coors.fill(val);
    }
    void swap(multi_vector& other){
        std::swap(coors, other.coors);
    }

    multi_vector& operator+=(const multi_vector& rhs)
    {
        std::transform(begin(), end(), rhs.begin(), begin(), std::plus<Key_T>());
        return *this;
    }

    multi_vector operator+(const multi_vector& rhs){
        multi_vector tmp;
        std::transform(begin(), end(), rhs.begin(), tmp.begin(), std::plus<Key_T>());
        return std::move(tmp);
    }


    multi_vector& operator-=(const multi_vector& rhs)
    {
        std::transform(begin(), end(), rhs.begin(), begin(), std::minus<Key_T>());
        return *this;
    }

    multi_vector operator-(const multi_vector& rhs){
        multi_vector tmp;
        std::transform(begin(), end(), rhs.begin(), tmp.begin(), std::minus<Key_T>());
        return std::move(tmp);
    }

    template<typename T>
    multi_vector& operator*=(const T& val){

        std::transform(begin(), end(), begin(),
                       std::bind1st(std::multiplies<T>(), val));
        return *this;
    }

    template<typename T>
    multi_vector operator*(const T& val) {
        multi_vector tmp;
        std::transform(begin(), end(), tmp.begin(),
                       std::bind1st(std::multiplies<T>(), val));
        return std::move(tmp);
    }

    template<typename T>
    multi_vector& operator/=(const T& val){

        std::transform(begin(), end(), begin(),
                       std::bind1st(std::divides<T>(), val));
        return *this;
    }

    template<typename T>
    multi_vector operator/(const T& val) {
        multi_vector tmp;
        std::transform(begin(), end(), tmp.begin(),
                       std::bind1st(std::divides<T>(), val));
        return std::move(tmp);
    }



    template<typename T>
    multi_vector& operator+=(const T& val){

        std::transform(begin(), end(), begin(),
                       std::bind1st(std::plus<T>(), val));
        return *this;
    }

    template<typename T>
    multi_vector operator+(const T& val) {
        multi_vector tmp;
        std::transform(begin(), end(), tmp.begin(),
                       std::bind1st(std::plus<T>(), val));
        return std::move(tmp);
    }


    template<typename T>
    multi_vector& operator-=(const T& val){

        std::transform(begin(), end(), begin(),
                       std::bind1st(std::minus<T>(), val));
        return *this;
    }

    template<typename T>
    multi_vector operator-(const T& val) {
        multi_vector tmp;
        std::transform(begin(), end(), tmp.begin(),
                       std::bind1st(std::minus<T>(), val));
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





    Key_T scalar_multiply(const multi_vector& rhs){
        Key_T tmp = std::inner_product(begin(), end(), rhs.begin(), 0.0);
        return std::move(tmp);
    }



    multi_vector& operator*=(const multi_vector& rhs){
        vector_multiply(*this, *this, rhs);
        return *this;
    }

    multi_vector operator*(const multi_vector& rhs){
        multi_vector tmp;
        vector_multiply(tmp, *this, rhs);
        return std::move(tmp);
    }

    bool operator==(const multi_vector& rhs){
       return std::equal(coors.begin(), coors.end(), rhs.begin());
    }

    bool operator!=(const multi_vector& rhs){
        return !((*this) == rhs);
    }

    bool operator< (const multi_vector& rhs){
        return !(coors >= rhs.coors);
    }

    void transform(const Matrix<Key_T, dim> & transform) {
        multi_vector<Key_T, dim + 1> transformedInstance = matrixVecMul(transform, expandToTramsform());
        std::copy(transformedInstance.begin(), transformedInstance.end() - 1, begin());
    }

};


template <typename T, size_t dim = 3>
multi_vector<T, dim + 1> matrixVecMul(const Matrix<T, dim> & matrix,
                                      const multi_vector<T, dim + 1> & vec) {
    multi_vector<T, dim + 1> result;
    for (size_t i = 0; i < dim; ++i) {
        for (size_t j = 0; j < dim; j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }
    return std::move(result);
}



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
