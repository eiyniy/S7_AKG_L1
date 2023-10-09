#pragma once

#include <optional>
#include <string>

class VertexIndexes
{
public:
    VertexIndexes();
    VertexIndexes(
        const int p_vertexId,
        const std::optional<int> p_tVertexId = std::nullopt,
        const std::optional<int> p_nVertexId = std::nullopt);
    VertexIndexes(std::string &str);

    const int cGetVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};
