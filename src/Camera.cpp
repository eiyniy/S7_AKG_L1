#include <Camera.hpp>
#include <Math.hpp>
#include <cmath>

Camera::Camera(
    const Matrix<4, 1> &_position,
    const Matrix<4, 1> &_target,
    Point &_resolution,
    const int _fov)
    : position(_position),
      target(_target),
      resolution(_resolution),
      fov(_fov * M_PI / 180) {}

void Camera::move(const Matrix<4, 1> &transition)
{
    target += transition;
    position += transition;
}

void Camera::rotateAround(
    const AxisName axisName,
    const Direction direction,
    const double step,
    bool &isReversed)
{
    auto cameraRelative = position - target;
    auto spherical = Math::decartToSpherical(cameraRelative);

    spherical.move(axisName, direction, step, isReversed);

    cameraRelative = Math::sphericalToDecart(spherical);
    position = cameraRelative + target;
}

void Camera::setResolution(const Point &newResolution)
{
    resolution = newResolution;
}

void Camera::setTarget(const Matrix<4, 1> &newTarget)
{
    target = newTarget;
}
