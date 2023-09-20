#include <NormalVertex.hpp>
#include <stdexcept>

using namespace std;

NormalVertex::NormalVertex(std::string &line) : BaseVertex(line)
{
    if (!v2.has_value() || !v3.has_value())
        throw invalid_argument("Invalid argument");
}

double NormalVertex::getI()
{
    return v1;
}

double NormalVertex::getJ()
{
    return v2.value();
}

double NormalVertex::getK()
{
    return v3.value();
}
