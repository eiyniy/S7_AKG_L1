#include <Math.hpp>

std::optional<int> Math::optStoi(const std::string &str)
{
    if (str.empty())
        return std::nullopt;
    else
        return stoi(str);
}

SphericalCoordinate Math::decartToSpherical(const CoordinateVector &cv)
{
    static const auto toDegree = 180 / M_PI;

    const auto r = sqrt(
        pow(cv.cGetX(), 2) +
        pow(cv.cGetY(), 2) +
        pow(cv.cGetZ(), 2));

    auto a = acos(cv.cGetZ() / r) * toDegree;
    auto b = atan(cv.cGetY() / cv.cGetX()) * toDegree;

    // NaN check
    if (a != a)
        a = 0.f;

    if (b != b)
        b = 0.f;

    return SphericalCoordinate(r, a, b);
}

CoordinateVector Math::sphericalToDecart(const SphericalCoordinate &sc)
{
    static const auto toRad = M_PI / 180;

    const auto radA = sc.a * toRad;
    const auto radB = sc.b * toRad;

    const auto sinA = sin(radA);

    const auto x = sc.r * sinA * cos(radB);
    const auto y = sc.r * sinA * sin(radB);
    const auto z = sc.r * cos(radA);

    return CoordinateVector(x, y, z, 1);
}
