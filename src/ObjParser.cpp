#include <ObjParser.hpp>
#include <optional>
#include <filesystem>
#include <iostream>

using namespace std;

ObjParser::ObjParser(string p_pathToFile)
{
    if (!std::filesystem::exists(p_pathToFile))
        throw std::runtime_error("Could not open file");

    pathToFile = p_pathToFile;

    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::runtime_error("Could not open file");
}

optional<string> ObjParser::getNextPart(string::iterator iter, string::iterator iterEnd)
{
    if (iter >= iterEnd)
        return {};

    auto iterSecond = iter;

    while (*iterSecond != ' ' && iterSecond != iterEnd)
        ++iterSecond;

    return string(iter, iterSecond);
}

void ObjParser::moveToNext(std::string::iterator *iter)
{
    while (**iter != ' ')
        (*iter)++;

    while (**iter == ' ')
        (*iter)++;
}

vector<Vertex> ObjParser::parseVertices()
{
    vector<Vertex> vertices;
    string line;

    while (getline(readStream, line) && !isVertex(line))
        continue;

    while (getline(readStream, line) && isVertex(line))
        vertices.push_back(parseVertex(line));

    return vertices;
}

bool ObjParser::isVertex(std::string &line)
{
    auto iter = line.begin();
    auto iterEnd = line.end();

    auto type = getNextPart(iter, iterEnd);

    if (type && type == "v")
        return true;
    else
        return false;
}

Vertex ObjParser::parseVertex(std::string &line)
{
    if (!isVertex(line))
        throw std::invalid_argument("Could not parse value");

    auto iter = line.begin();
    auto iterEnd = line.end();

    moveToNext(&iter);

    Vertex v;

    while (auto value = getNextPart(iter, iterEnd))
    {
        v.append(stod(value.value()));
        moveToNext(&iter);
    }

    return v;
}
