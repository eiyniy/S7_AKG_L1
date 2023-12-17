#pragma once

#include <Point.hpp>
#include <Matrix.hpp>

class Camera {
private:
    Matrix<4, 1> up;
    Matrix<4, 1> position;
    Matrix<4, 1> target;
    Point resolution;

    const int fov;

public:
    Camera(
            const Matrix<4, 1> &_up,
            const Matrix<4, 1> &_position,
            const Matrix<4, 1> &_target,
            Point &_resolution,
            int _fov);

    void move(const Matrix<4, 1> &transition);

    void rotateAround(
            AxisName axisName,
            Direction direction,
            double step);

    void setResolution(const Point &newResolution);

    void setTarget(const Matrix<4, 1> &newTarget);

    const Matrix<4, 1> &cGetUp() const;

    const Matrix<4, 1> &cGetPosition() const;

    const Matrix<4, 1> &cGetTarget() const;

    const Point &cGetResolution() const;

    int cGetFOV() const;

    double cGetAspect() const;
};

inline const Matrix<4, 1> &Camera::cGetUp() const {
    return up;
}

inline const Matrix<4, 1> &Camera::cGetPosition() const {
    return position;
}

inline const Matrix<4, 1> &Camera::cGetTarget() const {
    return target;
}

inline const Point &Camera::cGetResolution() const {
    return resolution;
}

inline int Camera::cGetFOV() const {
    return fov;
}

inline double Camera::cGetAspect() const {
    return ((double) resolution.cGetX()) / resolution.cGetY();
}
