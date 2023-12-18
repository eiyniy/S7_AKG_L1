#include <EarClipper.hpp>
#include <cmath>

std::vector<Polygon> EarClipper::triangulate(
    const std::vector<VertexIds> &indexes,
    const std::vector<Matrix<4, 1>> &vertices)
{
    std::vector<std::pair<Matrix<4, 1>, VertexIds>> polygonVertices;

    const int polygonSize = (int)indexes.size();
    for (int i = 0; i != polygonSize; ++i)
    {
        polygonVertices.emplace_back(vertices.at(i), indexes.at(i));
    }

    return triangulate(polygonVertices);
}

std::vector<Polygon> EarClipper::triangulate(std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices)
{
    std::vector<Polygon> result;

    while (polygonVertices.size() >= 3)
        result.emplace_back(clipEar(polygonVertices));

    return result;
}

bool EarClipper::isConvexVertex(
    const Matrix<4, 1> &vertex,
    const Matrix<4, 1> &prevVertex,
    const Matrix<4, 1> &nextVertex)
{
    const auto v1 = prevVertex - vertex;
    const auto v2 = nextVertex - vertex;

    const auto crossProduct = v1.vectorMultiply(v2);
    const auto sin = crossProduct.getLength() / (v1.getLength() * v2.getLength());

    return sin > 0;
}

Polygon EarClipper::clipEar(std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices)
{
    int i = 0;
    const int size = (int)polygonVertices.size();
    for (auto it = polygonVertices.cbegin(); it != polygonVertices.cend(); ++it, ++i)
    {
        auto itPrev = it;
        if (i > 0)
            itPrev--;
        else
            itPrev = polygonVertices.cend() - 1;

        auto itNext = it;
        if (size - i > 1)
            itNext++;
        else
            itNext = polygonVertices.cbegin();

        if (!isConvexVertex(
                it->first,
                itPrev->first,
                itNext->first))
            continue;

        if (isPointsInside(*it,
                           *itPrev,
                           *itNext,
                           polygonVertices))
            continue;

        const Polygon result{{it->second,
                              itNext->second,
                              itPrev->second}};

        polygonVertices.erase(it);

        return result;
    }
}

bool EarClipper::isPointsInside(
    const std::pair<Matrix<4, 1>, VertexIds> &v1,
    const std::pair<Matrix<4, 1>, VertexIds> &v2,
    const std::pair<Matrix<4, 1>, VertexIds> &v3,
    const std::vector<std::pair<Matrix<4, 1>, VertexIds>> &polygonVertices)
{
    bool result = false;

    const auto bigArea = area(v1.first, v2.first, v3.first);

    for (const auto &vertex : polygonVertices)
    {
        if (vertex.second.cGetVertexId() == v1.second.cGetVertexId() ||
            vertex.second.cGetVertexId() == v2.second.cGetVertexId() ||
            vertex.second.cGetVertexId() == v3.second.cGetVertexId())
            continue;

        const auto area1 = area(vertex.first, v2.first, v3.first);
        const auto area2 = area(v1.first, vertex.first, v3.first);
        const auto area3 = area(v1.first, v2.first, vertex.first);

        result |= bigArea == area1 + area2 + area3;

        if (result)
            break;
    }

    return result;
}
