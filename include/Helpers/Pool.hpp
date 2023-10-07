#pragma once

#include <list>
#include <iostream>

template <typename T>
class Pool
{
private:
    Pool() : freeList(), count(0), polledCount(0) {}
    ~Pool();

    std::list<T *> freeList;
    int count;
    int polledCount;

public:
    static Pool<T> &getInstance()
    {
        static Pool<T> instance;
        return instance;
    }

    T *get()
    {
        if (freeList.empty())
        {
            ++count;
            return new T();
        }

        ++polledCount;
        auto obj = freeList.back();
        freeList.pop_back();
        return obj;
    }

    inline void put(T *obj)
    {
        freeList.emplace_back(obj);
    }
};

template <typename T>
inline Pool<T>::~Pool()
{
    std::cout << "New obj count: " << count << std::endl;
    std::cout << "Pooled obj count: " << polledCount << std::endl;
    for (T *el : freeList)
        delete el;
}
