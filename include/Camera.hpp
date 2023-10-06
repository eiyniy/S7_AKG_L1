#pragma once

#include <CoordinateVector.hpp>
#include <Types.hpp>

class Camera
{
private:
    CoordinateVector position;
    CoordinateVector target;
    Dot resolution;
    int fov;

public:
    Camera(const CoordinateVector &p_position, const CoordinateVector &p_target, const Dot &p_resolution, const int p_fov);

    const CoordinateVector &cGetPosition() const;
    const CoordinateVector &cGetTarget() const;
    const Dot &cGetResolution() const;
    const int getFOV() const;
    const double getAspect() const;

    CoordinateVector &getPosition();
    CoordinateVector &getTarget();
    Dot &getResolution();
};
