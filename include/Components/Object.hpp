#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <Triangle.hpp>
#include <Camera.hpp>
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
        const std::optional<Texture> &_diffuseMap,
        const std::optional<Texture> &_normalMap,
        const std::optional<Texture> &_mraoMap,
        const std::optional<Texture> &_emissiveMap);

    void move(const Matrix<4, 1> &transition);

    const std::vector<Matrix<4, 1>> &cGetVertices() const;

    const std::vector<Matrix<4, 1>> &cGetTVertices() const;

    const std::vector<Matrix<4, 1>> &cGetNVertices() const;

    const std::vector<Triangle> &cGetPolygons() const;

    const std::optional<Texture> &cGetDiffuseMap() const;

    const std::optional<Texture> &cGetNormalMap() const;

    const std::optional<Texture> &cGetMRAOMap() const;

    const std::optional<Texture> &cGetEmissiveMap() const;

    std::vector<Triangle> &getPolygons();

    void convertToDrawable(const Camera &camera);

    const std::vector<Matrix<4, 1>> &cGetDrawable() const;

    const Matrix<4, 1> &getCenter();

    const Matrix<4, 1> &getMaxXZ();

    const Matrix<4, 1> &getMinXZ();

private:
    void calcGeometricParams();

    std::vector<Matrix<4, 1>> vertices;
    const std::vector<Matrix<4, 1>> nVertices;
    const std::vector<Matrix<4, 1>> tVertices;
    std::vector<Triangle> polygons;

    const std::optional<Texture> diffuseMap;
    const std::optional<Texture> normalMap;
    const std::optional<Texture> mraoMap;
    const std::optional<Texture> emissiveMap;

    std::optional<Matrix<4, 1>> center;
    std::optional<Matrix<4, 1>> maxXZ, minXZ;

    std::vector<Matrix<4, 1>> drawable;
};

inline const std::vector<Matrix<4, 1>> &Object::cGetDrawable() const
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

inline const std::optional<Texture> &Object::cGetDiffuseMap() const
{
    return diffuseMap;
}

inline const std::optional<Texture> &Object::cGetNormalMap() const
{
    return normalMap;
}

inline const std::optional<Texture> &Object::cGetMRAOMap() const
{
    return mraoMap;
}

inline const std::optional<Texture> &Object::cGetEmissiveMap() const
{
    return emissiveMap;
}

inline std::vector<Triangle> &Object::getPolygons()
{
    return polygons;
}
