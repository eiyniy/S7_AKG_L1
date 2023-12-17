#pragma once

#include <Matrix.hpp>

class BaseLightSource {
public:
    [[nodiscard]] virtual Matrix<4, 1> getLightDirection(const Matrix<4, 1> &target) const = 0;
};
