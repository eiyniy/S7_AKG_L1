#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class TextureVertex : public BaseVertex
{
public:
    TextureVertex(std::string &line);

    double getU();
    std::optional<double> getV();
    std::optional<double> getW();
};
