#include <TextureVertex.hpp>
#include <ObjParser.hpp>

TextureVertex::TextureVertex(std::string &line, ParseType parseType) : BaseVertex(line, parseType) {}

double TextureVertex::getU()
{
    return this->at(0);
}

std::optional<double> TextureVertex::getV()
{
    if (this->size() < 2)
        return {};

    return this->at(1);
}

std::optional<double> TextureVertex::getW()
{
    if (this->size() < 3)
        return {};

    return this->at(2);
}
