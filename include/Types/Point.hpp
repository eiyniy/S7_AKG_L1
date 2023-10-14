#pragma once

class Point
{
public:
    Point(const int p_x, const int p_y);

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