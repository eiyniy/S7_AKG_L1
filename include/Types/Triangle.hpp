#pragma once

#include <SFML/Graphics.hpp>
#include <VertexIds.hpp>
#include <Types.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <Matrix.hpp>
#include <Material.hpp>

class Triangle
{
public:
    sf::Color color;

    Triangle(
        const std::vector<VertexIds> &indexes,
        const std::string &_materialName);

    static std::vector<Triangle> parseAndTriangulate(
        const std::string &line,
        const std::vector<Matrix<4, 1>> &vertices,
        const std::string &materialName);

    const Matrix<4, 1> &getNormal(const std::vector<Matrix<4, 1>> &vertices);
    const Matrix<4, 1> &getCenter(const std::vector<Matrix<4, 1>> &vertices);

    [[nodiscard]] const std::string &cGetMaterialName() const;

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds &cGetVertexIds(int i) const;

private:
    int vertexIndexesCount;

    std::array<VertexIds, 3> values;

    const std::string materialName;
    // TODO: use static default material with grey color and etc.

    std::optional<Matrix<4, 1>> normal;
    std::optional<Matrix<4, 1>> center;

    static std::vector<VertexIds> parseInner(const std::string &line);
};

inline const std::string &Triangle::cGetMaterialName() const
{
    return materialName;
}
