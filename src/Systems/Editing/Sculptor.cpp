#include <Sculptor.hpp>
#include <BarycentricRasterizer.hpp>
#include <Matrix.hpp>
#include <iostream>

Sculptor::Sculptor(const int _radius)
    : radius(_radius) {}

void Sculptor::createGraph(Object *object)
{
    graph = Graph(object->cGetDrawable(), object->cGetPolygons());

    circle.setOutlineThickness(1);
    circle.setFillColor(sf::Color::Transparent);
    circle.setRadius(radius);
    circle.setOutlineColor(sf::Color::Red);
}

void Sculptor::pull(
    std::vector<Vector<4>> &vertices,
    const Point &mousePos,
    const Vector<4> &direction)
{
    const auto affectedVerticesIds = graph.getAffectedVerticesIds(mousePos, radius);
    // std::cout << "Sculptor log: " << std::endl;
    std::cout << "Affected Vertices count: " << affectedVerticesIds->size() << std::endl;

    for (auto &&vertexId : *affectedVerticesIds)
        vertices[vertexId] += direction;
}

void Sculptor::pull(
    Object *object,
    const int triangleId,
    const Point mousePos)
{
    // std::cout << "      " << triangleId << std::endl;

    auto &vertices = object->getVertices();
    auto polygon = object->getPolygons().at(triangleId);

    const auto normal = polygon.getFlatNormal(vertices);

    const auto vId0 = polygon.cGetVertexIds(0).cGetVertexId() - 1;
    const auto vId1 = polygon.cGetVertexIds(1).cGetVertexId() - 1;
    const auto vId2 = polygon.cGetVertexIds(2).cGetVertexId() - 1;

    // std::cout << vertices[vId0].cGetX() << std::endl;

    vertices[vId0] += normal;
    vertices[vId1] += normal;
    vertices[vId2] += normal;

    // std::cout << vertices[vId0].cGetX() << std::endl;
}
