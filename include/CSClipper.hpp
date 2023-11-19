#pragma once

#include <Point.hpp>
#include <Polygon.hpp>
#include <Vertex.hpp>

enum class ClipLineResult {
    Invisible,
    Nothing,
    First,
    Second,
    Both
};

class CSClipper {
private:
    static const int INSIDE = 0; // 0000
    static const int LEFT = 1; // 0001
    static const int RIGHT = 2; // 0010
    static const int BOTTOM = 4; // 0100
    static const int TOP = 8; // 1000

    const int xMax, yMax, xMin, yMin;

    [[nodiscard]]
    int computeCode(double x, double y) const;

    [[nodiscard]]
    static Point findIntersection(
            const Point &line1P1, const Point &line1P2,
            const Point &line2P1, const Point &line2P2);

public:
    CSClipper(int xMax, int yMax, int xMin, int yMin);

    ClipLineResult clipLine(int &x1, int &y1, int &x2, int &y2) const;

    /*
    void clipPolygon(
            Polygon &polygon,
            std::vector<Vertex> &vertices) const;
    */
};
