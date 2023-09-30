#include <ObjInfo.hpp>

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

const std::vector<Vertex> &ObjInfo::getVertices() const
{
    return vertices;
}

const std::vector<TextureVertex> &ObjInfo::getTVertices() const
{
    return tVertices;
}

const std::vector<NormalVertex> &ObjInfo::getNVertices() const
{
    return nVertices;
}

const std::vector<Polygon> &ObjInfo::getPolygons() const
{
    return polygons;
}
