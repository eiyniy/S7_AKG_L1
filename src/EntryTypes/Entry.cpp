#include <Entry.hpp>
#include <iostream>

using namespace std;

template <typename T>
void Entry<T>::append(T value)
{
    this->push_back(value);
}

template <typename T>
void Entry<T>::print()
{
    for (T el : *this)
        cout << el << "; ";

    cout << endl;
}

// this code is necessary for the Entry<T> to work properly.
template class Entry<double>;
template class Entry<int>;
