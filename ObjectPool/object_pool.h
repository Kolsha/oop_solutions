#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
#include <iostream>
#include <cstring>

template<typename T>
class ObjectPool
{
protected:
    size_t obj_size = sizeof(T);
    size_t size;
    T *objs;
    char *is_free;

public:
    ObjectPool(const size_t _size){
        if(_size < 1){
            return ;
        }
        size = _size;

        try{
            objs = (T*)new char[sizeof(T) * size];
            is_free = new char[size];
            std::memset(is_free, 1, size);
        }
        catch(...){
            delete objs;
            delete is_free;
        }
    }

    template<class ...Args> T &alloc(Args&&... args){
        for(size_t i = 0; i < size; i++){
            if(is_free[i]){
                is_free[i] = 0;
                new (objs + (i * obj_size)) T(args...);
                return * (objs + (i * obj_size));
            }
        }
        throw;
    }
    void free(T &obj){
        T *obj_ptr = &obj;
        for(size_t i = 0; i < size; i++){
            if(!is_free[i] && obj_ptr == (objs + (i * obj_size))){
                (objs + (i * obj_size))->~T();
                is_free[i] = 1;
                return ;
            }
        }
    }

    ~ObjectPool(){
        for(size_t i = 0; i < size; i++){
            if(!is_free[i]){
                (objs + (i * obj_size))->~T();
            }
        }
        delete objs;
        delete is_free;
    }
};

#endif // OBJECT_POOL_H
