#include <TextureVertex.hpp>
#include <ObjParser.hpp>

TextureVertex::TextureVertex(std::string &line) : BaseVertex(line) {}

double TextureVertex::getU()
{
    return v1;
}

std::optional<double> TextureVertex::getV()
{
    return v2;
}

std::optional<double> TextureVertex::getW()
{
    return v3;
}
