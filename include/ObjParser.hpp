#pragma once

#include <Vertex.hpp>
#include <string>
#include <optional>
#include <fstream>
#include <vector>

class ObjParser
{
public:
    ObjParser(std::string p_pathToFile);

    vector<Vertex> parseVertices(std::ifstream stream);
    bool isVertex(std::string line);
    Vertex parseVertex(std::string line);
    std::optional<std::string> getNextPart(std::string::iterator iter, std::string::iterator iterEnd);
    void moveToNext(std::string::iterator *iter);

private:
    std::ifstream readStream;
    std::string pathToFile;
};