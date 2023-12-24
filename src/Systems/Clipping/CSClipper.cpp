#include <cmath>
#include <CSClipper.hpp>

int CSClipper::computeCode(const double x, const double y) const
{
    int code = INSIDE;

    if (x < xMin) // to the left of rectangle
        code |= LEFT;
    else if (x > xMax) // to the right of rectangle
        code |= RIGHT;
    if (y < yMin) // below the rectangle
        code |= TOP;
    else if (y > yMax) // above the rectangle
        code |= BOTTOM;

    return code;
}

CSClipper::CSClipper(
    const int xMax, const int yMax,
    const int xMin, const int yMin)
    : xMax(xMax), yMax(yMax),
      xMin(xMin), yMin(yMin) {}

ClipLineResult CSClipper::clipLine(int &x1, int &y1, int &x2, int &y2) const
{
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);

    // Initialize line as outside the rectangular window
    ClipLineResult result = ClipLineResult::Nothing;

    while (true)
    {
        if (code1 == 0 && code2 == 0)
        {
            // If both endpoints lie within rectangle
            break;
        }
        else if (code1 & code2)
        {
            // If both endpoints are outside rectangle, in same region
            result = ClipLineResult::Invisible;
            break;
        }
        else
        {
            // Some segment of line lies within the rectangle
            int code_out;
            double x, y;

            // At least one endpoint is outside the rectangle, pick it.
            code_out = code1 != 0 ? code1 : code2;

            // Find intersection point;
            // y = y1 + slope * (x - x1),
            // x = x1 + (1 / slope) * (y - y1)

            /*
            Point intersection;

            if (code_out & TOP) {
                intersection = findIntersection(
                        {x1, y1}, {x2, y2},
                        {INT_MIN, INT_MAX}, {INT_MAX, INT_MIN});
            } else if (code_out & BOTTOM) {
                intersection = findIntersection(
                        {x1, y1}, {x2, y2},
                        {INT_MIN, INT_MAX}, {INT_MAX, INT_MAX});
            } else if (code_out & RIGHT) {
                intersection = findIntersection(
                        {x1, y1}, {x2, y2},
                        {INT_MAX, INT_MIN}, {INT_MAX, INT_MAX});
            } else if (code_out & LEFT) {
                intersection = findIntersection(
                        {x1, y1}, {x2, y2},
                        {INT_MIN, INT_MIN}, {INT_MIN, INT_MAX});
            }

            if ((code_out & TOP) || (code_out & BOTTOM) || (code_out & RIGHT) || (code_out & LEFT)) {
                x = intersection.cGetX();
                y = intersection.cGetY();
            }
             */

            if (code_out & TOP)
            {
                x = x1 + (x2 - x1) * (yMin - y1) / (double)(y2 - y1);
                y = yMin;
            }
            else if (code_out & BOTTOM)
            {
                x = x1 + (x2 - x1) * (yMax - y1) / (double)(y2 - y1);
                y = yMax;
            }
            else if (code_out & RIGHT)
            {
                y = y1 + (y2 - y1) * (xMax - x1) / (double)(x2 - x1);
                x = xMax;
            }
            else if (code_out & LEFT)
            {
                y = y1 + (y2 - y1) * (xMin - x1) / (double)(x2 - x1);
                x = xMin;
            }

            // Now intersection point x, y is found
            // We replace point outside rectangle
            // by intersection point
            if (code_out == code1)
            {
                x1 = floor(x);
                y1 = floor(y);

                code1 = computeCode(x1, y1);
                result = result == ClipLineResult::Nothing ? ClipLineResult::First : ClipLineResult::Both;
            }
            else
            {
                x2 = floor(x);
                y2 = floor(y);

                code2 = computeCode(x2, y2);
                result = result == ClipLineResult::Nothing ? ClipLineResult::Second : ClipLineResult::Both;
            }

            int a = 4;
        }
    }

    return result;
}

/*
void CSClipper::clipPolygon(
        Triangle &polygon,
        std::vector<Vertex> &vertices) const {
    const int polygonSize = polygon.cGetVertexIdsCount();

    bool allVisible = true;
    for (int i = 0; i < polygonSize; ++i)
        allVisible &= vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1).IsVisible();

    if (allVisible)
        return;

    std::vector<VertexIds> newVertexIds;
    newVertexIds.reserve(polygonSize);

    for (int i = 0; i < polygonSize; ++i) {
        const int k = (i + 1) % polygonSize;

        const auto vertex1 = vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);
        const auto vertex2 = vertices.at(polygon.cGetVertexIds(k).cGetVertexId() - 1);

        auto point1 = Converter::vertexToPoint(vertex1);
        auto point2 = Converter::vertexToPoint(vertex2);

        const auto result = clipLine(point1.getX(), point1.getY(), point2.getX(), point2.getY());

        switch (result) {
            case ClipLineResult::Invisible:
                continue;
            case ClipLineResult::Nothing:
                newVertexIds.emplace_back(polygon.cGetVertexIds(i).cGetVertexId());
                newVertexIds.emplace_back(polygon.cGetVertexIds(k).cGetVertexId());
                break;
            case ClipLineResult::First:
                vertices.emplace_back(point1.cGetX(), point1.cGetY(), 0);

                newVertexIds.emplace_back(vertices.size());
                newVertexIds.emplace_back(polygon.cGetVertexIds(k).cGetVertexId());
                break;
            case ClipLineResult::Second:
                vertices.emplace_back(point2.cGetX(), point2.cGetY(), 0);

                newVertexIds.emplace_back(polygon.cGetVertexIds(i).cGetVertexId());
                newVertexIds.emplace_back(vertices.size());
                break;
            case ClipLineResult::Both:
                vertices.emplace_back(point1.cGetX(), point1.cGetY(), 0);
                vertices.emplace_back(point2.cGetX(), point2.cGetY(), 0);

                newVertexIds.emplace_back(vertices.size() - 1);
                newVertexIds.emplace_back(vertices.size());
                break;
        }
    }

    if (newVertexIds.size() >= 3)
        polygon = Triangle(newVertexIds);
}
*/

Point CSClipper::findIntersection(
    const Point &line1P1, const Point &line1P2,
    const Point &line2P1, const Point &line2P2)
{
    const long a = (long)line1P1.cGetX() * (long)line1P2.cGetY() - (long)line1P1.cGetY() * (long)line1P2.cGetX();
    const long b = (long)line2P1.cGetX() * (long)line2P2.cGetY() - (long)line2P1.cGetY() * (long)line2P2.cGetX();

    const long denominator =
        ((long)line1P1.cGetX() - (long)line1P2.cGetX()) * ((long)line2P1.cGetY() - (long)line2P2.cGetY()) -
        ((long)line1P1.cGetY() - (long)line1P2.cGetY()) * ((long)line2P1.cGetX() - (long)line2P2.cGetX());

    const int x = (a * ((long)line2P1.cGetX() - (long)line2P2.cGetX()) -
                   b * ((long)line1P1.cGetX() - (long)line1P2.cGetX())) /
                  denominator;
    const int y = (a * ((long)line2P1.cGetY() - (long)line2P2.cGetY()) -
                   b * ((long)line1P1.cGetY() - (long)line1P2.cGetY())) /
                  denominator;

    return {x, y};
}
