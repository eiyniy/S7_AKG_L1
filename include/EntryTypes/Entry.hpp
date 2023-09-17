#pragma once

#include <vector>
#include <string>

template <class T>
class Entry : public std::vector<T>
{
public:
    void append(T value);
    void print();
};
