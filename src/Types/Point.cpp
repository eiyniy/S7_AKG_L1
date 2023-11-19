#include <Point.hpp>

Point::Point() : x{}, y{} {};

Point::Point(const int _x, const int _y)
        : x(_x), y(_y) {}

Point::Point(const Point &point)
        : Point(point.x, point.y) {}

Point &Point::operator=(const Point &point) {
    if (this == &point)
        return *this;

    x = point.x;
    y = point.y;

    return *this;
}
