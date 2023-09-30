#include <BaseVertex.hpp>
#include <ObjParser.hpp>
#include <Timer.hpp>
#include <MatrixStaticStorage.hpp>
#include <array>
#include <stdexcept>

BaseVertex::BaseVertex(const double p_v1, const double p_v2, const double p_v3, const double p_v4)
    : v1(p_v1), v2(p_v2), v3(p_v3), v4(p_v4) {}

BaseVertex::BaseVertex(const BaseVertex &bv)
    : v1(bv.v1), v2(bv.v2), v3(bv.v3), v4(bv.v4) {}

BaseVertex::BaseVertex(const std::string &line)
{
    auto entryType = ObjParser::getEntryType(line);
    if (entryType != EntryType::Vertex &&
        entryType != EntryType::TextureVertex &&
        entryType != EntryType::NormalVertex)
        throw std::logic_error("Could not parse value");

    std::optional<std::string> strPart;
    static auto accumulator = std::array<std::optional<double>, 4>();

    auto iter = line.cbegin();
    auto iterEnd = line.cend();

    ObjParser::getNextPart(&iter, iterEnd, ' ');

    int i = 0;
    while ((strPart = ObjParser::getNextPart(&iter, line.end(), ' ')))
    {
        accumulator[i] = stod(strPart.value());
        ++i;
    }

    if (i < 1)
        throw std::logic_error("Can't parse value");

    v1 = accumulator[0].value(),
    v2 = accumulator[1].value_or(0),
    v3 = accumulator[2].value_or(0);
    v4 = accumulator[3].value_or(1);

    accumulator.fill(std::nullopt);
}

BaseVertex BaseVertex::fromMatrix(const Matrix &m)
{
    return BaseVertex(m.getValue(0, 0), m.getValue(0, 1), m.getValue(0, 2));
}

BaseVertex::operator CoordinateVector() const
{
    return CoordinateVector(v1, v2, v3, v4);
}
