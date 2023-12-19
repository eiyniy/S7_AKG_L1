#pragma once

#include <BaseLightingModel.hpp>

class LambertModel : public BaseLightingModel {
public:
    [[nodiscard]] double getLightIntensity(
            const Matrix<4, 1> &normal,
            const Matrix<4, 1> &lightDirection,
            const Matrix<4, 1> &sightDirection) const override;
};
