#pragma once

#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <vector>

class ObjInfo
{
public:
    ObjInfo();

    void addVertex(const Vertex &vertex);
    void addTVertex(const TextureVertex &tVertex);
    void addNVertex(const NormalVertex &nVertex);
    void addPolygon(const Polygon &polygon);

    std::vector<Vertex> &getVertices();

    const std::vector<Vertex> &cGetVertices() const;
    const std::vector<TextureVertex> &cGetTVertices() const;
    const std::vector<NormalVertex> &cGetNVertices() const;
    const std::vector<Polygon> &cGetPolygons() const;

private:
    std::vector<Vertex> vertices;
    std::vector<TextureVertex> tVertices;
    std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;
};
