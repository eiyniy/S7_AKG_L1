#include <ObjInfo.hpp>

void ObjInfo::addVertex(Vertex vertex)
{
    vertices.push_back(vertex);
}

void ObjInfo::addTVertex(TextureVertex tVertex)
{
    tVertices.push_back(tVertex);
}

void ObjInfo::addNVertex(NormalVertex nVertex)
{
    nVertices.push_back(nVertex);
}

void ObjInfo::addPolygon(Polygon polygon)
{
    polygons.push_back(polygon);
}

std::vector<Vertex> &ObjInfo::getVertices()
{
    return vertices;
}

std::vector<TextureVertex> &ObjInfo::getTVertices()
{
    return tVertices;
}

std::vector<NormalVertex> &ObjInfo::getNVertices()
{
    return nVertices;
}

std::vector<Polygon> &ObjInfo::getPolygons()
{
    return polygons;
}
