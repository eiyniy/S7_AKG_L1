#include <NormalVertex.hpp>
#include <array>

NormalVertex::NormalVertex(const double i, const double j, const double k)
    : BaseVertex(i, j, k) {}

NormalVertex NormalVertex::parse(const std::string &line)
{
    auto acc = BaseVertex::parse(line);
    return NormalVertex(acc[0], acc[1], acc[2]);
}

const double NormalVertex::getI() const
{
    return v1;
}

const double NormalVertex::getJ() const
{
    return v2;
}

const double NormalVertex::getK() const
{
    return v3;
}
