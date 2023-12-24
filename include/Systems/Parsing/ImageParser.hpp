#pragma once

#include <string>
#include <Texture.hpp>
#include <CImg.h>
#include <SFML/Graphics.hpp>

namespace cimg = cimg_library;

template <typename T>
class ImageParser
{
    static_assert(std::is_same<T, Matrix<4, 1>>::value || std::is_same<T, sf::Color>::value,
                  "You only can use sf::Color or Matrix<4, 1> here");

private:
    const std::string path;

public:
    ImageParser(const std::string &_path);

    Texture<T> parse() const;
};
