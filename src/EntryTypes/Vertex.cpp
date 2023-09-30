#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex(const std::string &line) : BaseVertex(line) {}

Vertex::Vertex(const BaseVertex &bVertex) : BaseVertex(bVertex) {}

const double Vertex::getX() const
{
    return v1;
}

const double Vertex::getY() const
{
    return v2;
}

const double Vertex::getZ() const
{
    return v3;
}

const std::optional<double> Vertex::getW() const
{
    return v4;
}
