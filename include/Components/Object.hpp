#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <Triangle.hpp>
#include <Camera.hpp>
#include <DrawableVertex.hpp>
#include <Matrix.hpp>
#include <Texture.hpp>

class Object
{
public:
    Object(
        const std::vector<Matrix<4, 1>> &_vertices,
        const std::vector<Matrix<4, 1>> &_tVertices,
        const std::vector<Matrix<4, 1>> &_nVertices,
        const std::vector<Triangle> &_polygons,
        const std::optional<Texture<sf::Color>> &_diffuseMap,
        const std::optional<Texture<Matrix<4, 1>>> &_normalMap,
        const std::optional<Texture<Matrix<4, 1>>> &_mraoMap,
        const std::optional<Texture<sf::Color>> &_emissiveMap);

    void move(const Matrix<4, 1> &transition);

    const std::vector<Matrix<4, 1>> &cGetVertices() const;

    const std::vector<Matrix<4, 1>> &cGetTVertices() const;

    const std::vector<Matrix<4, 1>> &cGetNVertices() const;

    const std::vector<Triangle> &cGetPolygons() const;

    const std::optional<Texture<sf::Color>> &cGetDiffuseMap() const;

    const std::optional<Texture<Matrix<4, 1>>> &cGetNormalMap() const;

    const std::optional<Texture<Matrix<4, 1>>> &cGetMRAOMap() const;

    const std::optional<Texture<sf::Color>> &cGetEmissiveMap() const;

    std::vector<Triangle> &getPolygons();

    void convertToDrawable(const Camera &camera);

    const std::vector<DrawableVertex> &cGetDrawable() const;

    const Matrix<4, 1> &getCenter();

    const Matrix<4, 1> &getMaxXZ();

    const Matrix<4, 1> &getMinXZ();

private:
    void calcGeometricParams();

    std::vector<Matrix<4, 1>> vertices;
    const std::vector<Matrix<4, 1>> nVertices;
    const std::vector<Matrix<4, 1>> tVertices;
    std::vector<Triangle> polygons;

    const std::optional<Texture<sf::Color>> diffuseMap;
    const std::optional<Texture<Matrix<4, 1>>> normalMap;
    const std::optional<Texture<Matrix<4, 1>>> mraoMap;
    const std::optional<Texture<sf::Color>> emissiveMap;

    std::optional<Matrix<4, 1>> center;
    std::optional<Matrix<4, 1>> maxXZ, minXZ;

    std::vector<DrawableVertex> drawable;
};

inline const std::vector<DrawableVertex> &Object::cGetDrawable() const
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

inline const std::vector<Triangle> &Object::cGetPolygons() const
{
    return polygons;
}

inline const std::optional<Texture<sf::Color>> &Object::cGetDiffuseMap() const
{
    return diffuseMap;
}

inline const std::optional<Texture<Matrix<4, 1>>> &Object::cGetNormalMap() const
{
    return normalMap;
}

inline const std::optional<Texture<Matrix<4, 1>>> &Object::cGetMRAOMap() const
{
    return mraoMap;
}

inline const std::optional<Texture<sf::Color>> &Object::cGetEmissiveMap() const
{
    return emissiveMap;
}

inline std::vector<Triangle> &Object::getPolygons()
{
    return polygons;
}
