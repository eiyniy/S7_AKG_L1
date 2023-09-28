#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex(std::string &line)
    : BaseVertex(line, EntryType::Vertex) {}

double Vertex::getX()
{
    return v1;
}

double Vertex::getY()
{
    return v2;
}

double Vertex::getZ()
{
    return v3;
}

std::optional<double> Vertex::getW()
{
    return v4;
}
