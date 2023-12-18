#include <SHClipper.hpp>
#include <Converter.hpp>
#include <VertexIds.hpp>

void SHClipper::clip(
        Polygon &polygon,
        std::vector<DrawableVertex> &vertices,
        const std::vector<Point> &clipper) {
    const int clipperSize = (int) clipper.size();

    for (int i = 0; i < clipperSize; ++i) {
        const int k = (i + 1) % clipperSize;

        clipByEdge(polygon, vertices, clipper[i], clipper[k]);
    }
}

void SHClipper::clipByEdge(
        Polygon &polygon,
        std::vector<DrawableVertex> &vertices,
        const Point &edgeP1, const Point &edgeP2) {
    const int polygonSize = polygon.cGetVertexIdsCount();

    // bool allVisible = true;
    // for (int i = 0; i < polygonSize; ++i)
        // allVisible &= vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1).IsVisible();

    // if (allVisible)
        // return;

    std::vector<VertexIds> newVertexIds;
    newVertexIds.reserve(polygonSize);

    for (int i = 0; i < polygonSize; ++i) {
        const int k = (i + 1) % polygonSize;

        const auto &vertex1 = vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1);
        const auto &vertex2 = vertices.at(polygon.cGetVertexIds(k).cGetVertexId() - 1);

        const auto point1 = Converter::drawableVertexToPoint(vertex1);
        const auto point2 = Converter::drawableVertexToPoint(vertex2);

        const int p1Rel = findPointRelPos(point1, edgeP1, edgeP2);
        const int p2Rel = findPointRelPos(point2, edgeP1, edgeP2);

        if (p1Rel < 0 && p2Rel < 0) {
            newVertexIds.emplace_back(polygon.cGetVertexIds(k).cGetVertexId());
        } else if (p1Rel >= 0 && p2Rel < 0) {
            const auto intersection = findIntersection(point1, point2, edgeP1, edgeP2);

            vertices.emplace_back(intersection.cGetX(), intersection.cGetY(), 0);

            newVertexIds.emplace_back(vertices.size());
            newVertexIds.emplace_back(polygon.cGetVertexIds(k).cGetVertexId());
        } else if (p1Rel < 0) {
            const auto intersection = findIntersection(point1, point2, edgeP1, edgeP2);

            vertices.emplace_back(intersection.cGetX(), intersection.cGetY(), 0);

            newVertexIds.emplace_back(vertices.size());
        }
    }

    if (newVertexIds.size() >= 3)
        polygon = Polygon(newVertexIds);

/*
    for (int i = 0; i < polygon.cGetVertexIdsCount(); ++i)
        std::cout << '(' << vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1).cGetX()
                  << ", " << vertices.at(polygon.cGetVertexIds(i).cGetVertexId() - 1).cGetY()
                  << ") ";
    std::cout << std::endl;
*/
}

Point SHClipper::findIntersection(
        const Point &line1P1, const Point &line1P2,
        const Point &line2P1, const Point &line2P2) {
    const int a = line1P1.cGetX() * line1P2.cGetY() - line1P1.cGetY() * line1P2.cGetX();
    const int b = line2P1.cGetX() * line2P2.cGetY() - line2P1.cGetY() * line2P2.cGetX();

    const int denominator =
            (line1P1.cGetX() - line1P2.cGetX()) * (line2P1.cGetY() - line2P2.cGetY()) -
            (line1P1.cGetY() - line1P2.cGetY()) * (line2P1.cGetX() - line2P2.cGetX());

    const int x = (a * (line2P1.cGetX() - line2P2.cGetX()) - b * (line1P1.cGetX() - line1P2.cGetX())) / denominator;
    const int y = (a * (line2P1.cGetY() - line2P2.cGetY()) - b * (line1P1.cGetY() - line1P2.cGetY())) / denominator;

    return {x, y};
}
