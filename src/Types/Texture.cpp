#include <Texture.hpp>

Texture::Texture(
    const uint _width,
    const uint _height,
    const std::vector<Vector<4>> &_data)
    : width(_width), height(_height), data(_data) {}
