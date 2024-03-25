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
        const std::optional<std::string> &_materialName);

    static std::vector<Triangle> parseAndTriangulate(
        const std::string &line,
        const std::vector<Vector<4>> &vertices,
        const std::optional<std::string> &materialName);

    const Vector<4> &getFlatNormal(const std::vector<Vector<4>> &vertices);
    const Vector<4>  getPhongNormal(
        const std::vector<Vector<4>> &nVertices,
        const double b0, const double b1, const double b2);

    const Vector<4> &getCenter(const std::vector<Vector<4>> &vertices);

    [[nodiscard]] const std::optional<std::string> &cGetMaterialName() const;

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds &cGetVertexIds(int i) const;

private:
    int vertexIndexesCount;

    std::array<VertexIds, 3> values;

    const std::optional<std::string> materialName;
    // TODO: use static default material with grey color and etc.

    std::optional<Vector<4>> normal;
    std::optional<Vector<4>> center;

    static std::vector<VertexIds> parseInner(const std::string &line);
};

inline const std::optional<std::string> &Triangle::cGetMaterialName() const
{
    return materialName;
}
