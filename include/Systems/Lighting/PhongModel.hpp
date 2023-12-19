#pragma once

#include <BaseLightingModel.hpp>

class PhongModel : public BaseLightingModel
{
public:
    PhongModel(
        double _ambientCoeff,
        double _diffuseCoeff,
        double _specularCoeff,
        double _surfaceGlossCoeff);

    [[nodiscard]] double getLightIntensity(
        const Matrix<4, 1> &normal,
        const Matrix<4, 1> &lightDirection,
        const Matrix<4, 1> &sightDirection) const override;

private:
    const double ambientCoeff;
    const double diffuseCoeff;
    const double specularCoeff;
    const double surfaceGlossCoeff;
};
