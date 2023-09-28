#pragma once

#include <CoordinateVector.hpp>

class Camera
{
private:
    CoordinateVector position;

public:
    Camera(CoordinateVector p_position);

    CoordinateVector &getPosition();
};
