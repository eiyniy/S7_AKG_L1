#pragma once

#include <VertexIndexes.hpp>
#include <Types.hpp>
#include <string>
#include <vector>
#include <optional>

struct Values
{
    Values(std::vector<std::optional<VertexIndexes>> &values);
    Values();

    VertexIndexes v1;
    VertexIndexes v2;
    VertexIndexes v3;
    std::optional<VertexIndexes> v4;
};

class Polygon
{
public:
    Polygon(std::string &line);

private:
    PolygonStorageMode storageMode;

    std::optional<std::vector<VertexIndexes>> dValues;
    std::optional<Values> sValues;

    static std::vector<std::optional<VertexIndexes>> accumulator;

    void moveValuesToDynamic();
};
