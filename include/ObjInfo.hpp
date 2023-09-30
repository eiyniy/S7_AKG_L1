#pragma once

#include <Vertex.hpp>
#include <TextureVertex.hpp>
#include <NormalVertex.hpp>
#include <Polygon.hpp>
#include <vector>

class ObjInfo
{
public:
    void addVertex(const Vertex &vertex);
    void addTVertex(const TextureVertex &tVertex);
    void addNVertex(const NormalVertex &nVertex);
    void addPolygon(const Polygon &polygon);

    const std::vector<Vertex> &getVertices() const;
    const std::vector<TextureVertex> &getTVertices() const;
    const std::vector<NormalVertex> &getNVertices() const;
    const std::vector<Polygon> &getPolygons() const;

private:
    std::vector<Vertex> vertices;
    std::vector<TextureVertex> tVertices;
    std::vector<NormalVertex> nVertices;
    std::vector<Polygon> polygons;
};
