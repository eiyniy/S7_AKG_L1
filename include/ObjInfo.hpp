#pragma once

#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class ObjInfo
{
public:
    ObjInfo(
        const sf::Color &p_color,
        const int vReserve = 0,
        const int vtReserve = 0,
        const int vnReserve = 0,
        const int pReserve = 0);

    void addVertex(const Vertex &vertex);
    void addTVertex(const TextureVertex &tVertex);
    void addNVertex(const NormalVertex &nVertex);
    void addPolygon(const Polygon &polygon);

    std::vector<Vertex> &getVertices();

    const std::vector<Vertex> &cGetVertices() const;
    const std::vector<TextureVertex> &cGetTVertices() const;
    const std::vector<NormalVertex> &cGetNVertices() const;
    const std::vector<Polygon> &cGetPolygons() const;

    Vertex &getCenter();
    Vertex &getMaxXZ();
    Vertex &getMinXZ();

    const sf::Color &getColor() const;

private:
    void calcGeometricParams();

    std::vector<Vertex> vertices;
    std::vector<TextureVertex> tVertices;
    std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;

    std::optional<Vertex> center;
    std::optional<Vertex> maxXZ, minXZ;

    const sf::Color color;
};
