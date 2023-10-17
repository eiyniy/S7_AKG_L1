#pragma once

#include <VertexIndexes.hpp>
#include <Enums.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>

struct SValues
{
    SValues(const std::vector<VertexIndexes> &values);

    VertexIndexes v1;
    VertexIndexes v2;
    VertexIndexes v3;
    std::optional<VertexIndexes> v4;
};

class Polygon
{
public:
    Polygon(const std::vector<VertexIndexes> &indexes);

    static Polygon parse(const std::string &line);

    const int cGetVertexIndexesCount() const;
    const VertexIndexes &cGetVertexIndexes(const int i) const;

private:
    StorageMode storageMode;
    int vertexIndexesCount;

    std::optional<std::vector<VertexIndexes>> dValues;
    std::optional<SValues> sValues;

    void moveValuesToDynamic();
};
