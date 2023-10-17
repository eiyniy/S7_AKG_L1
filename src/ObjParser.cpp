#include <ObjParser.hpp>
#include <Enums.hpp>
#include <Timer.hpp>
#include <optional>
#include <filesystem>
#include <iostream>
#include <sstream>

ObjParser::ObjParser(const std::string &_pathToFile)
{
    if (!std::filesystem::exists(_pathToFile))
        throw std::logic_error("Could not open file");

    pathToFile = _pathToFile;

    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::logic_error("Could not open file");
}

#pragma region Static

std::optional<EntryType> ObjParser::getEntryType(const std::string &line)
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

std::optional<std::string> ObjParser::getNextPart(
    std::string::const_iterator *iter,
    std::string::const_iterator iterEnd,
    char divider,
    bool allowEmpty)
{
    if (*iter >= iterEnd)
        return std::nullopt;

    auto iterSecond = *iter;

    while (iterSecond < iterEnd && *iterSecond != divider)
        ++iterSecond;

    auto result = std::string(*iter, iterSecond);

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

ObjInfo *ObjParser::parseEntries(const std::string &fileContent)
{
    std::istringstream ss(fileContent);

    std::vector<Vertex> vertexes;
    std::vector<TextureVertex> tVertexes;
    std::vector<NormalVertex> nVertexes;
    std::vector<Polygon> polygons;

    auto iter = fileContent.cbegin();
    auto iterEnd = fileContent.cend();

    std::string line;

    while (getline(ss, line, '\n'))
    {
        auto type = getEntryType(line);
        if (!type.has_value())
            continue;

        switch (*type)
        {
        case EntryType::Vertex:
            vertexes.emplace_back(Vertex::parse(line));
            break;
        case EntryType::TextureVertex:
            tVertexes.emplace_back(TextureVertex::parse(line));
            break;
        case EntryType::NormalVertex:
            nVertexes.emplace_back(NormalVertex::parse(line));
            break;
        case EntryType::Polygon:
            polygons.emplace_back(Polygon::parse(line));
            break;
        }
    }

    readStream.clear();
    readStream.seekg(0, std::ios::beg);

    return new ObjInfo(
        vertexes,
        tVertexes,
        nVertexes,
        polygons,
        sf::Color::White);
}

std::unique_ptr<std::string> ObjParser::readFile()
{
    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = std::make_unique<std::string>(std::string(size, ' '));
    readStream.seekg(0);
    readStream.read(&((*buffer)[0]), size);

    return buffer;
}
