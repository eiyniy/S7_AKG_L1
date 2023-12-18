#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <Camera.hpp>
#include <DrawableVertex.hpp>
#include <Matrix.hpp>

class Object
{
public:
    Object(
        const std::vector<Matrix<4, 1>> &_vertices,
        const std::vector<Matrix<4, 1>> &_tVertices,
        const std::vector<Matrix<4, 1>> &_nVertices,
        const std::vector<Polygon> &_polygons,
        const sf::Color &_color);

    void move(const Matrix<4, 1> &transition);

    const std::vector<Matrix<4, 1>> &cGetVertices() const;

    const std::vector<Matrix<4, 1>> &cGetTVertices() const;

    const std::vector<Matrix<4, 1>> &cGetNVertices() const;

    const std::vector<Polygon> &cGetPolygons() const;

    std::vector<Polygon> &getPolygons();

    const Matrix<4, 1> &cGetShift() const;

    const sf::Color &cGetColor() const;

    void convertToDrawable(const Camera &camera);

    const std::vector<DrawableVertex> &cGetDrawable();

    const Matrix<4, 1> &getCenter();

    const Matrix<4, 1> &getMaxXZ();

    const Matrix<4, 1> &getMinXZ();

private:
    void calcGeometricParams();

    const std::vector<Matrix<4, 1>> vertices;
    const std::vector<Matrix<4, 1>> nVertices;
    const std::vector<Matrix<4, 1>> tVertices;
    std::vector<Polygon> polygons;

    const sf::Color color;

    std::optional<Matrix<4, 1>> center;
    std::optional<Matrix<4, 1>> maxXZ, minXZ;

    Matrix<4, 1> shift;

    std::vector<DrawableVertex> drawable;
};

inline const Matrix<4, 1> &Object::cGetShift() const
{
    return shift;
}

inline const sf::Color &Object::cGetColor() const
{
    return color;
}

inline const std::vector<DrawableVertex> &Object::cGetDrawable()
{
    return drawable;
}

inline const std::vector<Matrix<4, 1>> &Object::cGetVertices() const
{
    return vertices;
}

inline const std::vector<Matrix<4, 1>> &Object::cGetTVertices() const
{
    return tVertices;
}

inline const std::vector<Matrix<4, 1>> &Object::cGetNVertices() const
{
    return nVertices;
}

inline const std::vector<Polygon> &Object::cGetPolygons() const
{
    return polygons;
}

inline std::vector<Polygon> &Object::getPolygons()
{
    return polygons;
}
