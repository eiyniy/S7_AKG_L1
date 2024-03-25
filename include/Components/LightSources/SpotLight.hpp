#pragma once

#include <BaseLightSource.hpp>

class SpotLight : public BaseLightSource
{
public:
    explicit SpotLight(Vector<4> &_position);

    Vector<4> getLightDirection(const Vector<4> &target) const override;

private:
    Vector<4> position;
};
