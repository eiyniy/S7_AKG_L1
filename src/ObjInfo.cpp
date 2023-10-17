#include <optional>
#include <ObjInfo.hpp>

ObjInfo::ObjInfo(
    const std::vector<Vertex> &_vertices,
    const std::vector<TextureVertex> &_tVertices,
    const std::vector<NormalVertex> &_nVertices,
    const std::vector<Polygon> &_polygons,
    const sf::Color &_color)
    : color(_color),
      vertices(_vertices),
      tVertices(_tVertices),
      nVertices(_nVertices),
      polygons(_polygons) {}

const std::vector<Vertex> &ObjInfo::cGetVertices() const
{
    return vertices;
}

const std::vector<TextureVertex> &ObjInfo::cGetTVertices() const
{
    return tVertices;
}

const std::vector<NormalVertex> &ObjInfo::cGetNVertices() const
{
    return nVertices;
}

const std::vector<Polygon> &ObjInfo::cGetPolygons() const
{
    return polygons;
}

void ObjInfo::calcGeometricParams()
{
    double cx = 0, cy = 0, cz = 0;

    auto first = vertices.cbegin();
    double xMax = first->cGetX(),
           xMin = first->cGetX(),
           zMax = first->cGetZ(),
           zMin = first->cGetZ();

    for (auto &&vertex : vertices)
    {
        const auto x = vertex.cGetX();
        const auto y = vertex.cGetY();
        const auto z = vertex.cGetZ();

        if (x > xMax)
            xMax = x;
        else if (x < xMin)
            xMin = x;
        if (z > zMax)
            zMax = z;
        else if (z < zMin)
            zMin = z;

        cx += x;
        cy += y;
        cz += z;
    }

    const auto vCount = vertices.size();

    center = Vertex(cx / vCount, cy / vCount, cz / vCount);
    maxXZ = Vertex(xMax, 0, zMax);
    minXZ = Vertex(xMin, 0, zMin);
}

const Vertex &ObjInfo::getCenter()
{
    if (!center.has_value())
        calcGeometricParams();

    return *center;
}

const Vertex &ObjInfo::getMaxXZ()
{
    if (!maxXZ.has_value())
        calcGeometricParams();

    return *maxXZ;
}

const Vertex &ObjInfo::getMinXZ()
{
    if (!minXZ.has_value())
        calcGeometricParams();

    return *minXZ;
}

const sf::Color &ObjInfo::getColor() const
{
    return color;
}
