#include <NormalVertex.hpp>
#include <stdexcept>

NormalVertex::NormalVertex(std::string &line)
    : BaseVertex(line, EntryType::NormalVertex)
{
}

double NormalVertex::getI()
{
    return v1;
}

double NormalVertex::getJ()
{
    return v2;
}

double NormalVertex::getK()
{
    return v3;
}
