#include <Texture.hpp>

template <typename T>
Texture<T>::Texture(
    const uint &_width,
    const uint &_height,
    const std::vector<T> &_data)
    : width(_width), height(_height), data(_data)
{
}

template class Texture<Matrix<4, 1>>;
template class Texture<sf::Color>;