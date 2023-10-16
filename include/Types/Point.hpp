#pragma once

class Point
{
public:
    Point(const int _x, const int _y);
    Point(const Point &point);

    Point &operator=(const Point &point);

    const int cGetX() const;
    const int cGetY() const;

private:
    int x, y;
};

inline const int Point::cGetX() const
{
    return x;
}

inline const int Point::cGetY() const
{
    return y;
}