#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex(std::string &line)
    : BaseVertex(line, EntryType::Vertex) {}

double Vertex::getX() const
{
    return v1;
}

double Vertex::getY() const
{
    return v2;
}

double Vertex::getZ() const
{
    return v3;
}

std::optional<double> Vertex::getW() const
{
    return v4;
}
