#include <LambertModel.hpp>
#include <Converter.hpp>

double LambertModel::getLightIntensity(
        const NormalVertex &normalVertex,
        const Matrix<4, 1> &lightDirection) const {
    const auto mNormal = Converter::nVertexToMatrix(normalVertex);
    const auto lightCos = mNormal.scalarMultiply(lightDirection);

    return std::max(lightCos, 0.0);
}
