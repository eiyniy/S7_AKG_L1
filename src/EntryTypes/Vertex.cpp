#include <Vertex.hpp>
#include <ObjParser.hpp>
#include <iostream>

using namespace std;

Vertex::Vertex(std::string &line) : BaseVertex(line)
{
    if (!v2.has_value() || !v3.has_value())
        throw invalid_argument("Invalid argument");
}

double Vertex::getX()
{
    return v1;
}

double Vertex::getY()
{
    return v2.value();
}

double Vertex::getZ()
{
    return v3.value();
}

optional<double> Vertex::getW()
{
    return v4;
}