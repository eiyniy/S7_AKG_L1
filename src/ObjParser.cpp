#include <ObjParser.hpp>
#include <Types.hpp>
#include <optional>
#include <filesystem>
#include <functional>
#include <iostream>

using namespace std;

ObjParser::ObjParser(string p_pathToFile)
{
    if (!std::filesystem::exists(p_pathToFile))
        throw runtime_error("Could not open file");

    pathToFile = p_pathToFile;

    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw runtime_error("Could not open file");
}

#pragma region Static

optional<EntryType> ObjParser::getEntryType(std::string &line)
{
    auto iter = line.begin();

    auto type = getNextPart(&iter, line.end(), ' ');

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

optional<string> ObjParser::getNextPart(
    string::iterator *iter,
    string::const_iterator iterEnd,
    char divider,
    bool allowEmpty)
{
    if (*iter >= iterEnd)
        return std::nullopt;

    auto iterSecond = *iter;

    while (iterSecond < iterEnd && *iterSecond != divider)
        ++iterSecond;

    auto result = string(*iter, iterSecond);

    *iter = iterSecond;

    if (allowEmpty)
    {
        do
        {
            ++(*iter);
        } while (*iter < iterEnd && **iter != divider && **iter != '-' && !isdigit(**iter));
    }
    else
    {
        while (*iter < iterEnd && (**iter == divider || **iter == '\r'))
            ++(*iter);
    }

    return result;
}

#pragma endregion Static

ObjInfo *ObjParser::parseEntries(std::string &fileContent)
{
    auto info = new ObjInfo();

    auto iter = fileContent.begin();
    auto iterEnd = fileContent.cend();

    istringstream ss(fileContent);
    string line;

    while (getline(ss, line, '\n'))
        parseEntry(line, info);

    readStream.clear();
    readStream.seekg(0, ios::beg);

    return info;
}

void ObjParser::parseEntry(std::string &line, ObjInfo *result)
{
    optional<EntryType> type;
    if (!(type = getEntryType(line)))
        return;

    switch (type.value())
    {
    case EntryType::Vertex:
        result->addVertex(Vertex(line));
        break;
    case EntryType::TextureVertex:
        result->addTVertex(TextureVertex(line));
        break;
    case EntryType::NormalVertex:
        result->addNVertex(NormalVertex(line));
        break;
    case EntryType::Polygon:
        result->addPolygon(Polygon(line));
        break;
    default:
        break;
    }
}

std::string *ObjParser::readFile()
{
    readStream.seekg(0, ios::end);
    auto size = readStream.tellg();
    auto buffer = new string(size, ' ');
    readStream.seekg(0);
    readStream.read(&((*buffer)[0]), size);

    return buffer;
}
