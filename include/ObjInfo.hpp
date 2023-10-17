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
        const std::vector<Vertex> &_vertices,
        const std::vector<TextureVertex> &_tVertices,
        const std::vector<NormalVertex> &_nVertices,
        const std::vector<Polygon> &_polygons,
        const sf::Color &_color);

    const std::vector<Vertex> &cGetVertices() const;
    const std::vector<TextureVertex> &cGetTVertices() const;
    const std::vector<NormalVertex> &cGetNVertices() const;
    const std::vector<Polygon> &cGetPolygons() const;

    const Vertex &getCenter();
    const Vertex &getMaxXZ();
    const Vertex &getMinXZ();

    const sf::Color &getColor() const;

private:
    void calcGeometricParams();

    const std::vector<Vertex> vertices;
    const std::vector<TextureVertex> tVertices;
    const std::vector<NormalVertex> nVertices;
    const std::vector<Polygon> polygons;

    std::optional<Vertex> center;
    std::optional<Vertex> maxXZ, minXZ;

    const sf::Color color;
};
