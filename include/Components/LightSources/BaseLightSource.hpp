#pragma once

#include <Matrix.hpp>

class BaseLightSource {
public:
    [[nodiscard]] virtual Vector<4> getLightDirection(const Vector<4> &target) const = 0;
};
