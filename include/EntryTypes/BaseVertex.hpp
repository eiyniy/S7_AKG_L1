#pragma once

#include <Types.hpp>
#include <Matrix.hpp>
#include <CoordinateVector.hpp>
#include <string>
#include <optional>

class BaseVertex
{
public:
    BaseVertex(const double p_v1, const double p_v2, const double p_v3, const double p_v4 = 0);
    BaseVertex(const BaseVertex &bv);

    BaseVertex &operator=(const BaseVertex &bv);

    static std::array<double, 4> parse(const std::string &line);

    void log() const;

protected:
    double v1, v2, v3, v4;
};
