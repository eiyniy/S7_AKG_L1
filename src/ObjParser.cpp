#include <ObjParser.hpp>
#include <optional>
#include <filesystem>
#include <functional>
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

void ObjParser::moveToNext(string::iterator *iter)
{
    while (**iter != ' ')
        (*iter)++;

    while (**iter == ' ')
        (*iter)++;
}

vector<Vertex> *ObjParser::parseVertices()
{
    auto vertices = new vector<Vertex>;
    string line;

    while (getline(readStream, line) && !isVertex(line))
        continue;

    while (getline(readStream, line) && isVertex(line))
        vertices->push_back(Vertex(line));

    return vertices;
}

optional<EntryType> ObjParser::getEntryType(std::string &line)
{
    auto iter = line.begin();
    auto iterEnd = line.end();

    auto type = getNextPart(iter, iterEnd);

    if (type && type == "v")
        return EntryType::Vertex;
    else if (type && type == "vt")
        return EntryType::TextureVertex;
    else if (type && type == "vn")
        return EntryType::NormalVertex;
    else if (type && type == "f")
        return EntryType::Polygon;
    else
        return {};
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

template <typename T>
void ObjParser::parseEntry(std::string &line, T &result)
{
    auto type = getEntryType(line);
    if (!type)
        throw std::invalid_argument("Could not parse value");

    auto iter = line.begin();
    auto iterEnd = line.end();

    moveToNext(&iter);

    function<T(string &)> convertFunc;

    if (type == EntryType::Polygon)
        convertFunc = [](string &str)
        { return stoi(str); };
    else
        convertFunc = [](string &str)
        { return stod(str); };

    while (auto value = getNextPart(iter, iterEnd))
    {
        result.append(convertFunc(value.value()));
        moveToNext(&iter);
    }
}
