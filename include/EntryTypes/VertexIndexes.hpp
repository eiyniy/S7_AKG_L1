#pragma once

#include <optional>
#include <string>

class VertexIndexes
{
public:
    VertexIndexes(std::string &str);
    VertexIndexes();

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};
