#pragma once

class Point {
public:
    Point();

    Point(int _x, int _y);

    Point(const Point &point);

    Point &operator=(const Point &point);

    [[nodiscard]] int cGetX() const;

    [[nodiscard]] int cGetY() const;

    [[nodiscard]] int &getX();

    [[nodiscard]] int &getY();

private:
    int x, y;
};

inline int Point::cGetX() const {
    return x;
}

inline int Point::cGetY() const {
    return y;
}

inline int &Point::getX() {
    return x;
}

inline int &Point::getY() {
    return y;
}
