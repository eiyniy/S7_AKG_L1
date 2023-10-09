#include <optional>
#include <ObjInfo.hpp>

ObjInfo::ObjInfo() = default;

void ObjInfo::addVertex(const Vertex &vertex)
{
    vertices.push_back(vertex);
}

void ObjInfo::addTVertex(const TextureVertex &tVertex)
{
    tVertices.push_back(tVertex);
}

void ObjInfo::addNVertex(const NormalVertex &nVertex)
{
    nVertices.push_back(nVertex);
}

void ObjInfo::addPolygon(const Polygon &polygon)
{
    polygons.push_back(polygon);
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

Vertex &ObjInfo::cGetCenter()
{
    if (center.has_value())
        return *center;

    double cx = 0, cy = 0, cz = 0;

    for (auto &&vertex : vertices)
    {
        cx += vertex.cGetX();
        cy += vertex.cGetY();
        cz += vertex.cGetZ();
    }

    const auto vCount = vertices.size();

    center = Vertex(cx / vCount, cy / vCount, cz / vCount);

    return *center;
}
