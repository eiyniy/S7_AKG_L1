#pragma once

#include <BaseVertex.hpp>
#include <string>

class TextureVertex : public BaseVertex
{
public:
    TextureVertex(const double u, const double v = 0, const double w = 0);

    static TextureVertex parse(const std::string &line);

    const double getU() const;
    const double getV() const;
    const double getW() const;
};
