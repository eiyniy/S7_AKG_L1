#pragma once

#include <string>
#include <vector>
#include <optional>

struct Indexes
{
    Indexes(std::vector<std::optional<int>> &indexes);

    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};

class Polygon
{
public:
    Polygon(std::string &line);

private:
    std::vector<Indexes> values;
};
