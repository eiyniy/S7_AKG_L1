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

class ObjParser
{
public:
    explicit ObjParser(
        const std::string &_pathToObj,
        const std::optional<std::string> &_pathToDiffuseMap = std::nullopt,
        const std::optional<std::string> &_pathToNormalMap = std::nullopt,
        const std::optional<std::string> &_pathToMRAOMap = std::nullopt,
        const std::optional<std::string> &_pathToEmissiveMap = std::nullopt);

    Object *parse();

    static std::vector<std::string> splitByLines(const std::string &string);

    static std::optional<EntryType> getEntryType(const std::string &line);

    static std::optional<std::string> getNextPart(
        std::string::const_iterator *iter,
        std::string::const_iterator iterEnd,
        char divider,
        bool allowEmpty = false);

private:
    std::vector<Matrix<4, 1>> vertices;
    std::vector<Matrix<4, 1>> nVertices;
    std::vector<Matrix<4, 1>> tVertices;
    std::vector<Triangle> polygons;
    std::vector<std::string> polygonStrings;

    std::string pathToObj;
    std::optional<std::string> pathToDiffuseMap;
    std::optional<std::string> pathToNormalMap;
    std::optional<std::string> pathToMRAOMap;
    std::optional<std::string> pathToEmissiveMap;
    std::ifstream readStream;

    std::string readFile(const std::string &pathToFile);

    void parseEntry(const std::string &line);

    std::array<std::optional<double>, 4> parseAcc(const std::string &line);

    Matrix<4, 1> parseVertex(const std::array<std::optional<double>, 4> &acc);

    Matrix<4, 1> parseNVertex(const std::array<std::optional<double>, 4> &acc);

    Matrix<4, 1> parseTVertex(const std::array<std::optional<double>, 4> &acc);
};
