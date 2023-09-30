#include <TextureVertex.hpp>
#include <ObjParser.hpp>

TextureVertex::TextureVertex(const std::string &line) : BaseVertex(line) {}

const double TextureVertex::getU() const
{
    return v1;
}

const std::optional<double> TextureVertex::getV() const
{
    auto v = v2;

    if (v == 0)
        return std::nullopt;
    else
        return v;
}

const std::optional<double> TextureVertex::getW() const
{
    auto w = v3;

    if (w == 0)
        return std::nullopt;
    else
        return w;
}
