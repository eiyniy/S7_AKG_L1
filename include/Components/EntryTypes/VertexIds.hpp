#pragma once

#include <optional>
#include <string>

class VertexIds {
public:
    VertexIds();

    VertexIds(
            const int _vertexId,
            const std::optional<int> &_tVertexId = std::nullopt,
            const std::optional<int> &_nVertexId = std::nullopt);

    static VertexIds parse(const std::string &str);

    const int cGetVertexId() const;

    const std::optional<int> cGetNormalVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};
