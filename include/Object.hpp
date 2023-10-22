#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <Camera.hpp>

class Object
{
public:
    Object(
        const std::vector<Vertex> &_vertices,
        const std::vector<TextureVertex> &_tVertices,
        const std::vector<NormalVertex> &_nVertices,
        const std::vector<Polygon> &_polygons,
        const sf::Color &_color);

    void move(const Matrix<4, 1> &transition);

    const std::vector<Vertex> &cGetVertices() const;
    const std::vector<TextureVertex> &cGetTVertices() const;
    const std::vector<NormalVertex> &cGetNVertices() const;
    const std::vector<Polygon> &cGetPolygons() const;

    std::vector<Polygon> &getPolygons();

    const Matrix<4, 1> &cGetShift() const;
    const sf::Color &cGetColor() const;

    const std::vector<Vertex> getDrawable(const Camera &camera);

    const Vertex &getCenter();
    const Vertex &getMaxXZ();
    const Vertex &getMinXZ();

private:
    void calcGeometricParams();

    const std::vector<Vertex> vertices;
    const std::vector<TextureVertex> tVertices;
    const std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;

    const sf::Color color;

    std::optional<Vertex> center;
    std::optional<Vertex> maxXZ, minXZ;

    Matrix<4, 1> shift;

    std::vector<Vertex> drawable;
};

inline const Matrix<4, 1> &Object::cGetShift() const
{
    return shift;
}

inline const sf::Color &Object::cGetColor() const
{
    return color;
}
