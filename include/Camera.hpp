#pragma once

#include <Types.hpp>
#include <Matrix.hpp>

class Camera
{
private:
    Matrix<4, 1> position;
    Matrix<4, 1> target;
    Dot resolution;
    int fov;

public:
    Camera(const Matrix<4, 1> &p_position,
           const Matrix<4, 1> &p_target,
           Dot &p_resolution,
           const int p_fov);

    const Matrix<4, 1> &cGetPosition() const;
    const Matrix<4, 1> &cGetTarget() const;
    const Dot &cGetResolution() const;
    const int getFOV() const;
    const double getAspect() const;

    Matrix<4, 1> &getPosition();
    Matrix<4, 1> &getTarget();
    Dot &getResolution();
};
