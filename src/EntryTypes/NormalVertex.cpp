#include <NormalVertex.hpp>
#include <stdexcept>

NormalVertex::NormalVertex(std::string &line)
    : BaseVertex(line, EntryType::NormalVertex)
{
}

double NormalVertex::getI()
{
    return vector.getX();
}

double NormalVertex::getJ()
{
    return vector.getY();
}

double NormalVertex::getK()
{
    return vector.getZ();
}
