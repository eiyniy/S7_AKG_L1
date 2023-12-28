#pragma once

#include <vector>
#include <Matrix.hpp>
#include <SFML/Graphics.hpp>

class Texture
{
private:
    uint width;
    uint height;
    std::vector<Matrix<4, 1>> data;

public:
    Texture(
        const uint _width,
        const uint _height,
        const std::vector<Matrix<4, 1>> &_data);

    uint cGetWidth() const;
    uint cGetHeight() const;
    const std::vector<Matrix<4, 1>> &cGetData() const;
};

inline uint Texture::cGetWidth() const
{
    return width;
}

inline uint Texture::cGetHeight() const
{
    return height;
}

inline const std::vector<Matrix<4, 1>> &Texture::cGetData() const
{
    return data;
}
