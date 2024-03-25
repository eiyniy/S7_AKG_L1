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
    const Vector<4> &normal,
    const Vector<4> &diffuseColorMatrix,
    const Vector<4> &lightDirection,
    const Vector<4> &sightDirection,
    const Vector<4> &mrao) const
{
    const auto ambient = diffuseColorMatrix * ambientCoeff * mrao.cGetZ();
    const auto diffuse = diffuseColorMatrix * diffuseCoeff * std::max(normal.scalarMultiply(lightDirection), 0.0);
    auto reflectedLightDir = lightDirection - normal * 2 * normal.scalarMultiply(lightDirection);
    reflectedLightDir.normalize();
    const auto invSightDir = sightDirection * -1;
    const auto rv = std::max(reflectedLightDir.scalarMultiply(sightDirection), 0.0);
    const auto specular = Converter::colorToMatrix(sf::Color::White) * mrao.cGetX() * pow(rv, reflectionCoeff);

    return Converter::matrixToColor(ambient + diffuse + specular);
}

double PhongModel::cGetAmbientCoeff() const
{
    return ambientCoeff;
}
