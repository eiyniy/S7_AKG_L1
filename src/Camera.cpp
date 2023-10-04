#include <Camera.hpp>
#include <cmath>

Camera::Camera(
    const CoordinateVector &p_position,
    const CoordinateVector &p_target,
    const Dot p_resolution,
    const int p_fov)
    : position(p_position),
      target(p_target),
      resolution(p_resolution),
      fov(p_fov * M_PI / 180)
{
}

CoordinateVector &Camera::getPosition()
{
    return position;
}

CoordinateVector &Camera::getTarget()
{
    return target;
}

const CoordinateVector &Camera::cGetPosition() const
{
    return position;
}

const CoordinateVector &Camera::cGetTarget() const
{
    return target;
}

const Dot &Camera::cGetResolution() const
{
    return resolution;
}

Dot &Camera::getResolution()
{
    return resolution;
}

const int Camera::getFOV() const
{
    return fov;
}

const double Camera::getAspect() const
{
    return ((double)resolution.x) / resolution.y;
}
