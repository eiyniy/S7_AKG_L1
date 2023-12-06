#pragma once

#include <BaseLightingModel.hpp>

class LambertModel : public BaseLightingModel {
public:
    double getLightIntensity(
            const NormalVertex &normalVertex,
            const Matrix<4, 1> &lightDirection) const override;
};
