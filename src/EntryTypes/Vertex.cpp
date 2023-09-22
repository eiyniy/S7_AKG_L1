#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex(std::string &line)
    : BaseVertex(line, EntryType::Vertex) {}

std::optional<double> Vertex::getW()
{
    return v4;
}
