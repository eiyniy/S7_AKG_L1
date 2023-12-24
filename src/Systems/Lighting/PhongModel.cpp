#include <PhongModel.hpp>
#include <cmath>
#include <Converter.hpp>

PhongModel::PhongModel(
    const double _ambientCoeff,
    const double _diffuseCoeff,
    const double _reflectionCoeff)
    : ambientCoeff(_ambientCoeff),
      diffuseCoeff(_diffuseCoeff),
      reflectionCoeff(_reflectionCoeff) {}

sf::Color PhongModel::getLightIntensity(
    const Matrix<4, 1> &normal,
    const sf::Color &diffuseColor,
    const Matrix<4, 1> &lightDirection,
    const Matrix<4, 1> &sightDirection,
    const Matrix<4, 1> &mrao) const
{
    const auto colorMatrix = Converter::colorToMatrix(diffuseColor);
    const auto ambient = colorMatrix * ambientCoeff * (mrao.cGetZ() + 1) * 0.5;
    // const auto ambient = ambientCoeff;
    const auto diffuse = colorMatrix * diffuseCoeff * std::max(normal.scalarMultiply(lightDirection), 0.0);
    auto reflectedLightDir = lightDirection - normal * 2 * normal.scalarMultiply(lightDirection);
    reflectedLightDir.normalize();
    const auto invSightDir = sightDirection * -1;
    const auto rv = std::max(reflectedLightDir.scalarMultiply(sightDirection), 0.0);
    const auto specular = Converter::colorToMatrix(sf::Color::White) * (mrao.cGetX() + 1) * 0.5 * pow(rv, reflectionCoeff);

    return Converter::matrixToColor(ambient + diffuse + specular);
}

double PhongModel::cGetAmbientCoeff() const
{
    return ambientCoeff;
}
