#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex(const std::string &line) : BaseVertex(line) {}

Vertex::Vertex(const BaseVertex &bVertex) : BaseVertex(bVertex) {}

const double Vertex::cGetX() const
{
    return v1;
}

const double Vertex::cGetY() const
{
    return v2;
}

const double Vertex::cGetZ() const
{
    return v3;
}

const double Vertex::cGetW() const
{
    return v4;
}

double &Vertex::getX()
{
    return v1;
}

double &Vertex::getY()
{
    return v2;
}

double &Vertex::getZ()
{
    return v3;
}

double &Vertex::getW()
{
    return v4;
}
