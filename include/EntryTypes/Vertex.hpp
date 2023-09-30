#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex(const std::string &line);
    Vertex(const BaseVertex &bVertex);

    const double getX() const;
    const double getY() const;
    const double getZ() const;
    const std::optional<double> getW() const;
};
