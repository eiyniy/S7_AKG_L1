#include <optional>
#include <ObjInfo.hpp>

ObjInfo::ObjInfo(
    const int vReserve,
    const int vtReserve,
    const int vnReserve,
    const int pReserve)
{
    if (vReserve != 0)
        vertices.reserve(vReserve);
    if (vtReserve != 0)
        tVertices.reserve(vtReserve);
    if (vnReserve != 0)
        nVertices.reserve(vnReserve);
    if (pReserve != 0)
        polygons.reserve(pReserve);
}

void ObjInfo::addVertex(const Vertex &vertex)
{
    vertices.emplace_back(vertex);
}

void ObjInfo::addTVertex(const TextureVertex &tVertex)
{
    tVertices.emplace_back(tVertex);
}

void ObjInfo::addNVertex(const NormalVertex &nVertex)
{
    nVertices.emplace_back(nVertex);
}

void ObjInfo::addPolygon(const Polygon &polygon)
{
    polygons.emplace_back(polygon);
}

std::vector<Vertex> &ObjInfo::getVertices()
{
    return vertices;
}

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
        else if (z > zMin)
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

Vertex &ObjInfo::getCenter()
{
    if (!center.has_value())
        calcGeometricParams();

    return *center;
}

Vertex &ObjInfo::getMaxXZ()
{
    if (!maxXZ.has_value())
        calcGeometricParams();

    return *maxXZ;
}

Vertex &ObjInfo::getMinXZ()
{
    if (!minXZ.has_value())
        calcGeometricParams();

    return *minXZ;
}
