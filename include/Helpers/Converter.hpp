#include <typeinfo>
#include <CoordinateVector.hpp>
#include <Matrix.hpp>
#include <BaseVertex.hpp>
#include <Vertex.hpp>

class Converter
{
public:
    static Vertex coordinateVectorToVertex(CoordinateVector &value);
    static Matrix coordinateVectorToMatrix(CoordinateVector &value);
};
