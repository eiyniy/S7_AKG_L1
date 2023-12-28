#pragma once

#include <string>
#include <Texture.hpp>
#include <Types.hpp>
#include <CImg.h>
#include <SFML/Graphics.hpp>

namespace cimg = cimg_library;

class ImageParser
{
private:
    const std::string path;
    const TextureType type;

public:
    ImageParser(const std::string &_path, const TextureType _type);

    Texture parse() const;
};
