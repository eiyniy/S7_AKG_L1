#include <LambertModel.hpp>
#include <Converter.hpp>

sf::Color LambertModel::getLightIntensity(
    const Vector<4> &normal,
    const Vector<4> &diffuseColorMatrix,
    const Vector<4> &lightDirection,
    const Vector<4> &sightDirection,
    const Vector<4> &mrao) const
{
    const auto lightCos = normal.scalarMultiply(lightDirection);
    const auto intensity = std::max(lightCos, 0.0);

    return Converter::matrixToColor({intensity, intensity, intensity});
}
