#pragma once

#include <VertexIds.hpp>
#include <Enums.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>

struct SValues
{
    SValues(const std::vector<VertexIds> &values);

    VertexIds v1;
    VertexIds v2;
    VertexIds v3;
    std::optional<VertexIds> v4;
};

class Polygon
{
public:
    Polygon(const std::vector<VertexIds> &indexes);

    static Polygon parse(const std::string &line);

    const int cGetVertexIdsCount() const;
    const VertexIds &cGetVertexIds(const int i) const;

private:
    StorageMode storageMode;
    int vertexIndexesCount;

    std::optional<std::vector<VertexIds>> dValues;
    std::optional<SValues> sValues;

    void moveValuesToDynamic();
};
