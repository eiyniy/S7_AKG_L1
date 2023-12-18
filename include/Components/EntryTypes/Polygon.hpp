#pragma once

#include <SFML/Graphics.hpp>
#include <VertexIds.hpp>
#include <Enums.hpp>
#include <string>
#include <vector>
#include <optional>
#include <array>
#include <Vertex.hpp>
#include <Matrix.hpp>

struct SValues
{
    explicit SValues(const std::vector<VertexIds> &values);

    VertexIds v1;
    VertexIds v2;
    VertexIds v3;
    std::optional<VertexIds> v4;
};

class Polygon
{
public:
    sf::Color color;

    explicit Polygon(const std::vector<VertexIds> &indexes);

    static Polygon parse(const std::string &line);

    static std::vector<Polygon> parseAndTriangulate(
        const std::string &line,
        const std::vector<Matrix<4, 1>> &vertices);

    const Matrix<4, 1> &getNormal(const std::vector<Matrix<4, 1>> &vertices);
    const Matrix<4, 1> &getCenter(const std::vector<Matrix<4, 1>> &vertices);

    [[nodiscard]] int cGetVertexIdsCount() const;

    [[nodiscard]] const VertexIds &cGetVertexIds(int i) const;

private:
    StorageMode storageMode;
    int vertexIndexesCount;

    std::optional<std::vector<VertexIds>> dValues;
    std::optional<SValues> sValues;

    std::optional<Matrix<4, 1>> normal;
    std::optional<Matrix<4, 1>> center;

    void moveValuesToDynamic();

    static std::vector<VertexIds> parseInner(const std::string &line);
};
