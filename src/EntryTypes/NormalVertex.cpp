#include <NormalVertex.hpp>
#include <stdexcept>

NormalVertex::NormalVertex(const std::string &line) : BaseVertex(line) {}

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
