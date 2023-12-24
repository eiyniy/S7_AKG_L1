#pragma once

#include <vector>
#include <Matrix.hpp>
#include <SFML/Graphics.hpp>

template <typename T>
class Texture
{
    static_assert(std::is_same<T, Matrix<4, 1>>::value || std::is_same<T, sf::Color>::value,
                  "You only can use sf::Color or Matrix<4, 1> here");

private:
    uint width;
    uint height;
    std::vector<T> data;

public:
    Texture(
        const uint &_width,
        const uint &_height,
        const std::vector<T> &_data);

    uint cGetWidth() const;
    uint cGetHeight() const;
    const std::vector<T> &cGetData() const;
};

template <typename T>
inline uint Texture<T>::cGetWidth() const
{
    return width;
}

template <typename T>
inline uint Texture<T>::cGetHeight() const
{
    return height;
}

template <typename T>
inline const std::vector<T> &Texture<T>::cGetData() const
{
    return data;
}
