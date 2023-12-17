#pragma once

#include <optional>
#include <string>

class VertexIds {
public:
    VertexIds();

    explicit VertexIds(
            int _vertexId,
            const std::optional<int> &_tVertexId = std::nullopt,
            const std::optional<int> &_nVertexId = std::nullopt);

    static VertexIds parse(const std::string &str);

    [[nodiscard]] int cGetVertexId() const;

    [[nodiscard]] std::optional<int> cGetNormalVertexId() const;

    [[nodiscard]] std::optional<int> cGetTextureVertexId() const;

private:
    int vertexId;
    std::optional<int> tVertexId;
    std::optional<int> nVertexId;
};

inline int VertexIds::cGetVertexId() const {
    return vertexId;
}

inline std::optional<int> VertexIds::cGetNormalVertexId() const {
    return nVertexId;
}

inline std::optional<int> VertexIds::cGetTextureVertexId() const {
    return tVertexId;
}
