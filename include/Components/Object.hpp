#pragma once

#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>
#include <Triangle.hpp>
#include <Camera.hpp>
#include <Matrix.hpp>
#include <Texture.hpp>
#include <Material.hpp>

class Object
{
public:
    Object(
        const std::vector<Matrix<4, 1>> &_vertices,
        const std::vector<Matrix<4, 1>> &_tVertices,
        const std::vector<Matrix<4, 1>> &_nVertices,
        const std::vector<Triangle> &_polygons,
        std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> _materials);

    void move(const Matrix<4, 1> &transition);

    const std::vector<Matrix<4, 1>> &cGetVertices() const;

    const std::vector<Matrix<4, 1>> &cGetTVertices() const;

    const std::vector<Matrix<4, 1>> &cGetNVertices() const;

    const std::vector<Triangle> &cGetPolygons() const;

    const std::shared_ptr<const Material> cGetMaterial(const std::string &name) const;
    static const std::shared_ptr<const Material> getDefaultMaterial();

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

    std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> materials;
    static std::shared_ptr<const Material> defaultMaterial;

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

inline const std::shared_ptr<const Material> Object::cGetMaterial(const std::string &name) const
{
    if (!materials->contains(name))
        throw std::runtime_error("Can't get material");

    return materials->at(name);
}

inline const std::shared_ptr<const Material> Object::getDefaultMaterial()
{
    return defaultMaterial;
}

inline std::vector<Triangle> &Object::getPolygons()
{
    return polygons;
}
