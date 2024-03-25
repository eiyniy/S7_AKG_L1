#pragma once

#include <BaseLightSource.hpp>

class DirectLight : public BaseLightSource {
public:
    explicit DirectLight(Vector<4> &direction);

    Vector<4> getLightDirection(const Vector<4> &target) const override;

private:
    Vector<4> direction;
};
