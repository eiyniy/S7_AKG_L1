#include <optional>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <mutex>
#include <ObjParser.hpp>
#include <Types.hpp>
#include <Timer.hpp>
#include <ThreadPool.hpp>
#include <ImageParser.hpp>
#include <Globals.hpp>

ObjParser::ObjParser(
    const std::string &_pathToObj,
    const std::optional<std::string> &_pathToDiffuseMap,
    const std::optional<std::string> &_pathToNormalMap,
    const std::optional<std::string> &_pathToMRAOMap,
    const std::optional<std::string> &_pathToEmissiveMap)
{
    if (!std::filesystem::exists(_pathToObj) ||
        (_pathToDiffuseMap.has_value() && !std::filesystem::exists(*_pathToDiffuseMap)) ||
        (_pathToNormalMap.has_value() && !std::filesystem::exists(*_pathToNormalMap)) ||
        (_pathToMRAOMap.has_value() && !std::filesystem::exists(*_pathToMRAOMap)) ||
        (_pathToEmissiveMap.has_value() && !std::filesystem::exists(*_pathToEmissiveMap)))
        throw std::logic_error("Could not open file");

    pathToObj = _pathToObj;
    pathToDiffuseMap = _pathToDiffuseMap;
    pathToNormalMap = _pathToNormalMap;
    pathToMRAOMap = _pathToMRAOMap;
    pathToEmissiveMap = _pathToEmissiveMap;
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

Object *ObjParser::parse()
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    std::optional<Texture> diffuseMap;
    if (pathToDiffuseMap.has_value())
    {
        const ImageParser parser{*pathToDiffuseMap, TextureType::Diffuse};
        diffuseMap = parser.parse();
    }

    std::optional<Texture> normlMap;
    if (pathToNormalMap.has_value())
    {
        const ImageParser parser{*pathToNormalMap, TextureType::Normal};
        normlMap = parser.parse();
    }

    std::optional<Texture> mraoMap;
    if (pathToMRAOMap.has_value())
    {
        const ImageParser parser{*pathToMRAOMap, TextureType::MRAO};
        mraoMap = parser.parse();
    }

    std::optional<Texture> emissiveMap;
    if (pathToEmissiveMap.has_value())
    {
        const ImageParser parser{*pathToEmissiveMap, TextureType::Emissive};
        emissiveMap = parser.parse();
    }

    const auto fileContent = readFile(pathToObj);
    const auto lines = splitByLines(fileContent);

    for (const auto &line : lines)
        parseEntry(line);

    polygons.reserve(polygonStrings.size());

#pragma omp parallel for if (!_DEBUG)
    for (const auto &line : polygonStrings)
    {
        const auto triangulated = Triangle::parseAndTriangulate(line, vertices);
        for (const auto &polygon : triangulated)
        {
#pragma omp critical(new_triangle)
            polygons.emplace_back(polygon);
        }
    }

    const auto timeEnd = std::chrono::high_resolution_clock::now();
    auto parseTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    return new Object(
        vertices,
        tVertices,
        nVertices,
        polygons,
        diffuseMap,
        normlMap,
        mraoMap,
        emissiveMap);
}

void ObjParser::parseEntry(const std::string &line)
{
    const auto type = getEntryType(line);
    if (!type.has_value())
        return;

    std::array<std::optional<double>, 4> acc;
    if (type != EntryType::Polygon)
        acc = parseAcc(line);

    switch (*type)
    {
    case EntryType::Polygon:
        polygonStrings.emplace_back(line);
        break;
    case EntryType::Vertex:
        vertices.emplace_back(parseVertex(acc));
        break;
    case EntryType::NormalVertex:
        nVertices.emplace_back(parseNVertex(acc));
        break;
    case EntryType::TextureVertex:
        tVertices.emplace_back(parseTVertex(acc));
        break;
    }
}

std::array<std::optional<double>, 4> ObjParser::parseAcc(const std::string &line)
{
    const auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::logic_error("Could not parse value");

    std::optional<std::string> strPart;
    auto accumulator = std::array<std::optional<double>, 4>();

    auto iter = line.cbegin();
    const auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    for (; (strPart = ObjParser::getNextPart(&iter, line.end(), ' ')); ++i)
        accumulator[i] = std::stod(*strPart);

    if (i < 1)
        throw std::logic_error("Can't parse value");

    return accumulator;
}

Matrix<4, 1> ObjParser::parseVertex(const std::array<std::optional<double>, 4> &acc)
{
    return {*acc[0], *acc[1], *acc[2], acc[3].value_or(1)};
}

Matrix<4, 1> ObjParser::parseNVertex(const std::array<std::optional<double>, 4> &acc)
{
    return {*acc[0], *acc[1], *acc[2], 0};
}

Matrix<4, 1> ObjParser::parseTVertex(const std::array<std::optional<double>, 4> &acc)
{
    return {*acc[0], acc[1].value_or(0), acc[2].value_or(0), 1};
}

std::vector<std::string> ObjParser::splitByLines(const std::string &string)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{string};

    for (std::string line; std::getline(ss, line, '\n');)
        result.emplace_back(line);

    return result;
}

std::string ObjParser::readFile(const std::string &pathToFile)
{
    readStream.open(pathToFile);
    if (!readStream.is_open())
        throw std::logic_error("Could not open file");

    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = std::string(size, ' ');
    readStream.seekg(0);
    readStream.read(&(buffer[0]), size);
    readStream.close();

    return buffer;
}
