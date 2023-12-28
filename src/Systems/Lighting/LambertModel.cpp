#include <LambertModel.hpp>
#include <Converter.hpp>

sf::Color LambertModel::getLightIntensity(
    const Matrix<4, 1> &normal,
    const Matrix<4, 1> &diffuseColorMatrix,
    const Matrix<4, 1> &lightDirection,
    const Matrix<4, 1> &sightDirection,
    const Matrix<4, 1> &mrao) const
{
    const auto lightCos = normal.scalarMultiply(lightDirection);
    const auto intensity = std::max(lightCos, 0.0);

    return Converter::matrixToColor({intensity, intensity, intensity});
}
