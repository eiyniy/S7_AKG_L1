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

    static std::optional<EntryType> getEntryType(std::string &line);
    static std::optional<std::string> getNextPart(std::string::iterator iter, std::string::iterator iterEnd);
    static void moveToNext(std::string::iterator *iter);

private:
    std::ifstream readStream;
    std::string pathToFile;

    template <typename T>
    void parseEntry(std::string &line, T &result);
    bool isVertex(std::string &line);
};
