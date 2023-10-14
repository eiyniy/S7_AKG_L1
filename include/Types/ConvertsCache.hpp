#pragma once

#include <Matrix.hpp>
#include <Camera.hpp>
#include <optional>
#include <memory>
#include <map>

class ConvertsCache
{
public:
    ConvertsCache(
        Camera &p_camera,
        Matrix<4, 1> &p_up);

    void setChanged(const MatrixConvert key, const bool value);

    const std::unique_ptr<Matrix<4, 4>> getMoveConvert(std::unique_ptr<Matrix<4, 1>> objectShift = nullptr);
    const std::unique_ptr<Matrix<4, 4>> getViewConvert();
    const std::unique_ptr<Matrix<4, 4>> getProjectionConvert();
    const std::unique_ptr<Matrix<4, 4>> getViewportConvert();

private:
    Camera &camera;
    Matrix<4, 1> &up;

    std::map<MatrixConvert, bool> isConvertChanged;
    std::map<MatrixConvert, Matrix<4, 4>> convertCached;
};
