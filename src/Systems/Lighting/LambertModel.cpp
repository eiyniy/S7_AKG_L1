#include <LambertModel.hpp>
#include <Converter.hpp>

double LambertModel::getLightIntensity(
        const Matrix<4, 1> &normal,
        const Matrix<4, 1> &lightDirection) const {
    const auto lightCos = normal.scalarMultiply(lightDirection);

    return std::max(lightCos, 0.0);
}
