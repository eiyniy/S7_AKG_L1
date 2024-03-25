#pragma once

#include <Object.hpp>
#include <Types.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>
#include <memory>
#include <Matrix.hpp>
#include <Texture.hpp>
#include <BaseTextParser.hpp>

class ObjParser : public BaseTextParser
{
public:
    explicit ObjParser(const std::string &_pathToObj);

    Object *parse();

    static std::optional<ObjEntryType> getEntryType(const std::string &line);

private:
    std::vector<Vector<4>> vertices;
    std::vector<Vector<4>> nVertices;
    std::vector<Vector<4>> tVertices;
    std::vector<Triangle> polygons;
    std::vector<std::pair<std::string, std::optional<std::string>>> polygonsStringAndMaterial;

    std::string mtlPath;
    std::unique_ptr<const std::map<std::string, std::shared_ptr<const Material>>> materials;

    std::optional<std::string> currMaterialName;

    void parseEntry(const std::string &line);

    static std::array<std::optional<double>, 4> parseAcc(const std::string &line);

    Vector<4> parseVertex(const std::array<std::optional<double>, 4> &acc);

    Vector<4> parseNVertex(const std::array<std::optional<double>, 4> &acc);

    Vector<4> parseTVertex(const std::array<std::optional<double>, 4> &acc);
};
