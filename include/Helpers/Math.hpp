#pragma once

#include <optional>
#include <string>
#include <Types.hpp>
#include <CoordinateVector.hpp>

class Math
{
public:
    Math() = delete;

    static std::optional<int> optStoi(const std::string &str);
    static SphericalCoordinate decartToSpherical(const CoordinateVector &cv);
    static CoordinateVector sphericalToDecart(const SphericalCoordinate &sc);
};
