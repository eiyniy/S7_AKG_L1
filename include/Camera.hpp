#pragma once

#include <Point.hpp>
#include <Matrix.hpp>

class Camera
{
private:
    Matrix<4, 1> position;
    Matrix<4, 1> target;
    Point resolution;
    int fov;

public:
    Camera(const Matrix<4, 1> &_position,
           const Matrix<4, 1> &_target,
           Point &_resolution,
           const int _fov);

    const Matrix<4, 1> &cGetPosition() const;
    const Matrix<4, 1> &cGetTarget() const;
    const Point &cGetResolution() const;
    const int getFOV() const;
    const double getAspect() const;

    Matrix<4, 1> &getPosition();
    Matrix<4, 1> &getTarget();
    Point &getResolution();
};

inline Matrix<4, 1> &Camera::getPosition()
{
    return position;
}

inline Matrix<4, 1> &Camera::getTarget()
{
    return target;
}

inline const Matrix<4, 1> &Camera::cGetPosition() const
{
    return position;
}

inline const Matrix<4, 1> &Camera::cGetTarget() const
{
    return target;
}

inline const Point &Camera::cGetResolution() const
{
    return resolution;
}

inline Point &Camera::getResolution()
{
    return resolution;
}

inline const int Camera::getFOV() const
{
    return fov;
}

inline const double Camera::getAspect() const
{
    return ((double)resolution.cGetX()) / resolution.cGetY();
}
