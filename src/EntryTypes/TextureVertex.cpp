#include <TextureVertex.hpp>
#include <ObjParser.hpp>

TextureVertex::TextureVertex(std::string &line)
    : BaseVertex(line, EntryType::TextureVertex) {}

double TextureVertex::getU()
{
    return v1;
}

std::optional<double> TextureVertex::getV()
{
    auto v = v2;

    if (v == 0)
        return std::nullopt;
    else
        return v;
}

std::optional<double> TextureVertex::getW()
{
    auto w = v3;

    if (w == 0)
        return std::nullopt;
    else
        return w;
}
