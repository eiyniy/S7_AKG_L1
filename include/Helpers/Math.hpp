#pragma once

#include <optional>
#include <string>
#include <SphericalCoordinate.hpp>
#include <Matrix.hpp>

class Math
{
public:
    Math() = delete;

    static std::optional<int> optStoi(const std::string &str);
    static SphericalCoordinate decartToSpherical(const Vector<4> &cv);
    static Vector<4> sphericalToDecart(const SphericalCoordinate &sc);
};
