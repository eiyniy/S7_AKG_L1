#include <Vertex.hpp>
#include <iostream>

using namespace std;

void Vertex::append(double value)
{
    this->push_back(value);
}

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
    if (this->size() != 4)
        return {};

    return this->at(3);
}

void Vertex::print()
{
    cout << getX() << "; " << getY() << "; " << getZ() << "; ";

    if (auto w = getW())
        cout << w.value();
    else
        cout << "NOT W";

    cout << endl;
}
