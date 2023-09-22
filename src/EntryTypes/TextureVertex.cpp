#include <TextureVertex.hpp>
#include <ObjParser.hpp>

TextureVertex::TextureVertex(std::string &line)
    : BaseVertex(line, EntryType::TextureVertex) {}

double TextureVertex::getU()
{
    return vector.getX();
}

std::optional<double> TextureVertex::getV()
{
    auto v = vector.getY();

    if (v == 0)
        return std::nullopt;
    else
        return v;
}

std::optional<double> TextureVertex::getW()
{
    auto w = vector.getY();

    if (w == 0)
        return std::nullopt;
    else
        return w;
}
