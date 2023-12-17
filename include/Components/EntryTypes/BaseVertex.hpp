#pragma once

#include <Enums.hpp>
#include <Matrix.hpp>
#include <string>
#include <optional>

class BaseVertex {
public:
    BaseVertex() = default;

    BaseVertex(
            double _v1,
            double _v2,
            double _v3,
            double _v4 = 0);

    BaseVertex(const BaseVertex &bv);

    BaseVertex &operator=(const BaseVertex &bv);

    static std::array<double, 4> parse(const std::string &line);

    void log() const;

protected:
    double v1, v2, v3, v4;
};
