#pragma once

#include <VertexIndexes.hpp>
#include <Types.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>

struct SValues
{
    SValues(const std::array<std::optional<VertexIndexes>, 4> &values);

    VertexIndexes v1;
    VertexIndexes v2;
    VertexIndexes v3;
    std::optional<VertexIndexes> v4;
};

class Polygon
{
public:
    Polygon(const std::string &line);
    Polygon(const std::vector<VertexIndexes> &indexes);

    const int cGetVertexIndexesCount() const;
    const VertexIndexes &cGetVertexIndexes(const int i) const;

private:
    StorageMode storageMode;
    int vertexIndexesCount;

    std::optional<std::vector<VertexIndexes>> dValues;
    std::optional<SValues> sValues;

    void moveValuesToDynamic();
};
