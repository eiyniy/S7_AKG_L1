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
    const auto x = cv.cGetX();
    const auto y = cv.cGetZ();
    const auto z = cv.cGetY();

    static const auto toDegree = 180 / M_PI;

    const auto r = sqrt(
        pow(x, 2) +
        pow(y, 2) +
        pow(z, 2));

    auto a = acos(z / r) * toDegree;
    double b = atan(y / x) * toDegree;

    if (x <= 0)
        b += 180;
    else if (x >= 0 && y < 0)
        b += 360;

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
    const auto z = sc.r * sinA * sin(radB);
    const auto y = sc.r * cos(radA);

    return CoordinateVector(x, y, z, 1);
}
