#include <Types.hpp>

Dot::Dot(const int p_x, const int p_y)
    : x(p_x), y(p_y) {}

SphericalCoordinate::SphericalCoordinate(
    const double p_r,
    const double p_a,
    const double p_b)
    : r(p_r), a(p_a), b(p_b) {}
