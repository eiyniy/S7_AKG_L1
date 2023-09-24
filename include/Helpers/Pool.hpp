#pragma once

#include <list>

template <typename T>
class Pool
{
private:
    Pool() : freeList() {}

    std::list<T *> freeList;

public:
    static Pool<T> &getInstance()
    {
        static Pool<T> instance;
        return instance;
    }

    T *get()
    {
        if (freeList.empty())
            return new T();

        auto obj = freeList.back();
        freeList.pop_back();
        return obj;
    }

    inline void put(T *obj) { freeList.emplace_back(obj); }
};