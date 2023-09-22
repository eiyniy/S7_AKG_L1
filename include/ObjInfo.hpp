#pragma once

#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <vector>

class ObjInfo
{
public:
    void addVertex(Vertex vertex);
    void addTVertex(TextureVertex tVertex);
    void addNVertex(NormalVertex nVertex);
    void addPolygon(Polygon polygon);

    std::vector<Vertex> &getVertices();
    std::vector<TextureVertex> &getTVertices();
    std::vector<NormalVertex> &getNVertices();
    std::vector<Polygon> &getPolygons();

    void convertPolygons();

private:
    std::vector<Vertex> vertices;
    std::vector<TextureVertex> tVertices;
    std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;
};
