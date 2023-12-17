#include <optional>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <mutex>
#include <ObjParser.hpp>
#include <Enums.hpp>
#include <Timer.hpp>
#include <ThreadPool.hpp>

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

Object *ObjParser::parseEntries(const std::string &fileContent)
{
    const auto timeStart = std::chrono::high_resolution_clock::now();

    const auto lines = splitByLines(fileContent);

    /*
    // const int threadsCount = (unsigned int)ceil(lines.size() / 10000.f);
    const int threadsCount = std::min(
        (unsigned int)ceil(lines.size() / 10000.f),
        ThreadPool::getInstance().getThreadsCount());

    const double size = lines.size() / (double)threadsCount;

    for (int i = 0; i < threadsCount; ++i)
    {
        const int begin = floor(size * i);
        const int end = floor(size * (i + 1)) - 1;

        ThreadPool::getInstance().enqueue(
            [this, i, begin, end, &lines]()
            {
                for (int j = begin; j <= end; ++j)
                    parseEntry(lines[j]);
            });
    }

    ThreadPool::getInstance().waitAll();
    */

    // /*
    for (const auto &line : lines)
        parseEntry(line);
    // */

    for (const auto &line : polygonStrings)
    {
        const auto triangulated = Polygon::parseAndTriangulate(line, vertices);
        for (const auto &polygon : triangulated)
            polygons.emplace_back(polygon);
    }

    const auto timeEnd = std::chrono::high_resolution_clock::now();
    auto parseTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeStart).count();
    std::cout << "Parse time: " << parseTime << " ms" << std::endl;

    return new Object(
        vertices,
        tVertices,
        nVertices,
        polygons,
        sf::Color::White);
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

std::string ObjParser::readFile()
{
    readStream.seekg(0, std::ios::end);
    auto size = readStream.tellg();
    auto buffer = std::string(size, ' ');
    readStream.seekg(0);
    readStream.read(&(buffer[0]), size);
    readStream.close();

    return buffer;
}
