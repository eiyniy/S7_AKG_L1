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
        const std::vector<Vector<4>> &_vertices,
        const std::vector<Vector<4>> &_tVertices,
        const std::vector<Vector<4>> &_nVertices,
        const std::vector<Triangle> &_polygons,
        std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> _materials);

    void move(const Vector<4> &transition);

    const std::vector<Vector<4>> &cGetVertices() const;

    const std::vector<Vector<4>> &cGetTVertices() const;

    const std::vector<Vector<4>> &cGetNVertices() const;

    const std::vector<Triangle> &cGetPolygons() const;

    const std::shared_ptr<const Material> cGetMaterial(const std::string &name) const;
    static const std::shared_ptr<const Material> getDefaultMaterial();

    std::vector<Vector<4>> &getVertices();

    std::vector<Triangle> &getPolygons();

    void convertToDrawable(const Camera &camera);

    const std::vector<Vector<4>> &cGetDrawable() const;

    const Vector<4> &getCenter();

    const Vector<4> &getMaxXZ();

    const Vector<4> &getMinXZ();

private:
    void calcGeometricParams();

    std::vector<Vector<4>> vertices;
    const std::vector<Vector<4>> nVertices;
    const std::vector<Vector<4>> tVertices;
    std::vector<Triangle> polygons;

    std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> materials;
    static std::shared_ptr<const Material> defaultMaterial;

    std::optional<Vector<4>> center;
    std::optional<Vector<4>> maxXZ, minXZ;

    std::vector<Vector<4>> drawable;
};

inline const std::vector<Vector<4>> &Object::cGetDrawable() const
{
    return drawable;
}

inline const std::vector<Vector<4>> &Object::cGetVertices() const
{
    return vertices;
}

inline const std::vector<Vector<4>> &Object::cGetTVertices() const
{
    return tVertices;
}

inline const std::vector<Vector<4>> &Object::cGetNVertices() const
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

inline std::vector<Vector<4>> &Object::getVertices()
{
    return vertices;
}

inline std::vector<Triangle> &Object::getPolygons()
{
    return polygons;
}
