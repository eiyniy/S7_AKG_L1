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
    const Matrix<4, 1> &diffuseColorMatrix,
    const Matrix<4, 1> &lightDirection,
    const Matrix<4, 1> &sightDirection,
    const Matrix<4, 1> &mrao) const
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
