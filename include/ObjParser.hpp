#pragma once

#include <Vertex.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>

enum class EntryType
{
    Vertex,
    TextureVertex,
    NormalVertex,
    Polygon
};

class ObjParser
{
public:
    ObjParser(std::string p_pathToFile);

    std::vector<Vertex> *parseVertices();
    bool isVertex(std::string &line);
    void parseVertex(std::string &line, Vertex &result);
    std::optional<std::string> getNextPart(std::string::iterator iter, std::string::iterator iterEnd);
    void moveToNext(std::string::iterator *iter);

private:
    std::ifstream readStream;
    std::string pathToFile;
};