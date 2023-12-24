#pragma once

#include <SFML/Graphics.hpp>
#include <VertexIds.hpp>
#include <Types.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <Matrix.hpp>

class Triangle
{
public:
    sf::Color color;

    explicit Triangle(const std::vector<VertexIds> &indexes);

    static Triangle parse(const std::string &line);

    static std::vector<Triangle> parseAndTriangulate(
        const std::string &line,
        const std::vector<Matrix<4, 1>> &vertices);

    const Matrix<4, 1> &getNormal(const std::vector<Matrix<4, 1>> &vertices);
    const Matrix<4, 1> &getCenter(const std::vector<Matrix<4, 1>> &vertices);

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds &cGetVertexIds(int i) const;

private:
    int vertexIndexesCount;

    std::array<VertexIds, 3> values;

    std::optional<Matrix<4, 1>> normal;
    std::optional<Matrix<4, 1>> center;

    static std::vector<VertexIds> parseInner(const std::string &line);
};
