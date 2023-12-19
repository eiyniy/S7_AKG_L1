#include <PhongModel.hpp>
#include <cmath>

PhongModel::PhongModel(
    const double _ambientCoeff,
    const double _diffuseCoeff,
    const double _specularCoeff,
    const double _surfaceGlossCoeff)
    : ambientCoeff(_ambientCoeff),
      diffuseCoeff(_diffuseCoeff),
      specularCoeff(_specularCoeff),
      surfaceGlossCoeff(_surfaceGlossCoeff) {}

double PhongModel::getLightIntensity(
    const Matrix<4, 1> &normal,
    const Matrix<4, 1> &lightDirection,
    const Matrix<4, 1> &sightDirection) const
{
    const auto ambient = ambientCoeff;
    const auto diffuse = std::max(diffuseCoeff * normal.scalarMultiply(lightDirection), 0.0);
    const auto reflectedLightDir = lightDirection - normal * 2 * normal.scalarMultiply(lightDirection);
    const auto specular = std::max(specularCoeff * pow(reflectedLightDir.scalarMultiply(sightDirection), surfaceGlossCoeff), 0.0);

    return ambient + diffuse + specular;
}