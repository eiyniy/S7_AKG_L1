#include <typeinfo>
#include <CoordinateVector.hpp>
#include <Matrix.hpp>
#include <BaseVertex.hpp>
#include <Vertex.hpp>

class Converter
{
public:
    static Vertex cVectorToVertex(const CoordinateVector &value);
    static Matrix cVectorToMatrix(const CoordinateVector &value);
    static CoordinateVector vertexToCVector(const Vertex &value);
    static CoordinateVector matrixToCVector(const Matrix &value);
};
