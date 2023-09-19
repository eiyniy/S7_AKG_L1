#include <NormalVertex.hpp>

NormalVertex::NormalVertex(std::string &line, ParseType parseType) : BaseVertex(line, parseType) {}

double NormalVertex::getI()
{
    return this->at(0);
}

double NormalVertex::getJ()
{
    return this->at(1);
}

double NormalVertex::getK()
{
    return this->at(2);
}
