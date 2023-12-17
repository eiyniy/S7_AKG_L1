#pragma once

#include <Object.hpp>
#include <Enums.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>
#include <memory>
#include <Matrix.hpp>

class ObjParser
{
public:
    explicit ObjParser(const std::string &_pathToFile);

    std::string readFile();

    Object *parseEntries(const std::string &fileContent);

    void parseEntry(const std::string &line);

    std::array<std::optional<double>, 4> parseAcc(const std::string &line);

    Matrix<4, 1> parseVertex(const std::array<std::optional<double>, 4> &acc);

    Matrix<4, 1> parseNVertex(const std::array<std::optional<double>, 4> &acc);

    Matrix<4, 1> parseTVertex(const std::array<std::optional<double>, 4> &acc);

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
    std::vector<Polygon> polygons;
    std::vector<std::string> polygonStrings;

    std::string pathToFile;
    std::ifstream readStream;
};
