#pragma once

#include <optional>
#include <string>
#include <Types.hpp>
#include <Matrix.hpp>

class Math
{
public:
    Math() = delete;

    static std::optional<int> optStoi(const std::string &str);
    static SphericalCoordinate decartToSpherical(const Matrix<4, 1> &cv);
    static Matrix<4, 1> sphericalToDecart(const SphericalCoordinate &sc);
};
