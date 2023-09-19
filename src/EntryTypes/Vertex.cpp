#include <Vertex.hpp>
#include <ObjParser.hpp>
#include <iostream>

using namespace std;

Vertex::Vertex(std::string &line, ParseType parseType) : BaseVertex(line, parseType) {}

double Vertex::getX()
{
    return this->at(0);
}

double Vertex::getY()
{
    return this->at(1);
}

double Vertex::getZ()
{
    return this->at(2);
}

optional<double> Vertex::getW()
{
    if (this->size() < 4)
        return {};

    return this->at(3);
}