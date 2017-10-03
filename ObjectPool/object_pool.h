#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
#include <iostream>
#include <cstring>


class ObjectPoolException :public std::exception {};

class OutOfLimitException :public ObjectPoolException
{
public :const char *what() const noexcept {return "out of limit";}
};

template<typename T>
class ObjectPool
{
protected:
    size_t obj_size = sizeof(T);
    size_t sz;
    T *objs;
    char *is_free;

public:
    inline size_t size(){
        return sz;
    }

    ObjectPool(const size_t _size){
        if(_size < 1){
            return ;
        }
        sz = _size;

        try{
            objs = (T*)new char[sizeof(T) * sz];
            is_free = new char[sz];
            std::memset(is_free, 1, sz);
        }
        catch(...){
            delete objs;
            delete is_free;
        }
    }

    template<class ...Args> T &alloc(Args&&... args){
        for(size_t i = 0; i < sz; i++){
            if(is_free[i]){
                is_free[i] = 0;
                new (objs + (i * obj_size)) T(std::forward<Args>(args)...);
                return * (objs + (i * obj_size));
            }
        }
        throw OutOfLimitException();
    }
    void free(T &obj){
        T *obj_ptr = &obj;
        for(size_t i = 0; i < sz; i++){
            if(!is_free[i] && obj_ptr == (objs + (i * obj_size))){
                (objs + (i * obj_size))->~T();
                is_free[i] = 1;
                return ;
            }
        }
    }

    ~ObjectPool(){
        for(size_t i = 0; i < sz; i++){
            if(!is_free[i]){
                (objs + (i * obj_size))->~T();
            }
        }
        delete objs;
        delete is_free;
    }
};

#endif // OBJECT_POOL_H
