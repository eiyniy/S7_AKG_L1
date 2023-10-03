#include <ObjInfo.hpp>

ObjInfo::ObjInfo() {}

ObjInfo::ObjInfo(const ObjInfo &objInfo)
{
    vertices = std::vector(objInfo.vertices);
    tVertices = objInfo.tVertices;
    nVertices = objInfo.nVertices;
    polygons = objInfo.polygons;
}

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
