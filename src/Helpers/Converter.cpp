#include <Converter.hpp>
#include <MatrixStaticStorage.hpp>

Matrix Converter::cVectorToMatrix(const CoordinateVector &value)
{
    auto storage = MatrixStaticStorage<4, 1>::getNewPooled();
    auto mx = Matrix(storage, true);

    mx.getValue(0, 0) = value.cGetX();
    mx.getValue(1, 0) = value.cGetY();
    mx.getValue(2, 0) = value.cGetZ();
    mx.getValue(3, 0) = value.cGetW();

    return mx;
}

CoordinateVector Converter::vertexToCVector(const Vertex &value)
{
    return CoordinateVector(value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW());
}

CoordinateVector Converter::matrixToCVector(const Matrix &value)
{
    if (value.getCols() != 1 || value.getRows() != 4)
        throw std::invalid_argument("Could not convert value");

    return CoordinateVector(value.getValue(0, 0), value.getValue(1, 0), value.getValue(2, 0), value.getValue(3, 0));
}

Vertex Converter::cVectorToVertex(const CoordinateVector &value)
{
    return Vertex(value.cGetX(), value.cGetY(), value.cGetZ(), value.cGetW());
}
