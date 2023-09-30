#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class TextureVertex : public BaseVertex
{
public:
    TextureVertex(const std::string &line);

    const double getU() const;
    const std::optional<double> getV() const;
    const std::optional<double> getW() const;
};
