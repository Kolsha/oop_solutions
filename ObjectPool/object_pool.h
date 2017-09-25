#ifndef OBJECT_POOL_H
#define OBJECT_POOL_H
#include <iostream>
#include <vector>

template<typename T>
class ObjectPool
{
protected:
    struct ObjInfo{
        T *obj;
        bool is_free = true;
    };
    std::vector<ObjInfo> objs;
public:
    ObjectPool(const size_t size){
        if(size < 1){
            throw;
        }
        objs.reserve(size);
        for(size_t i = 0; i < size; i++){
            ObjInfo tmp;
            tmp.is_free = true;
            tmp.obj = (T*)malloc(sizeof(T));
            if(tmp.obj == nullptr){
                continue;//throw;
            }
            objs.push_back(std::move(tmp));
        }
    }

    template<class ...Args> T &alloc(Args&&... args){
        for(size_t i = 0; i < objs.size(); i++){
            if(objs[i].is_free){
                objs[i].is_free = false;
                new (objs[i].obj) T(args...);
                return *(objs[i].obj);
            }
        }
        throw;
    }
    void free(T &obj){
        T *obj_ptr = &obj;
        for(size_t i = 0; i < objs.size(); i++){
            if(!objs[i].is_free && obj_ptr == objs[i].obj){
                objs[i].obj->~T();
                return ;
            }
        }
    }

    ~ObjectPool(){
        for(size_t i = 0; i < objs.size(); i++){
            objs[i].obj->~T();
            T *obj_ptr = objs[i].obj;
            std::free(obj_ptr);
        }
    }
};

#endif // OBJECT_POOL_H
