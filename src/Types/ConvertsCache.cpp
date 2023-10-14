#include <ConvertsCache.hpp>
#include <iostream>

ConvertsCache::ConvertsCache(Camera &p_camera, Matrix<4, 1> &p_up)
    : camera(p_camera),
      up(p_up)
{
    for (int i = (int)MatrixConvert::Move; i <= (int)MatrixConvert::Viewport; ++i)
    {
        isConvertChanged[(MatrixConvert)i] = true;
        convertCached[(MatrixConvert)i] = Matrix<4, 4>();
    }
}

void ConvertsCache::setChanged(const MatrixConvert key, const bool value)
{
    isConvertChanged.at(key) = value;
}

const std::unique_ptr<Matrix<4, 4>> ConvertsCache::getMoveConvert(std::unique_ptr<Matrix<4, 1>> objectShift)
{
    if (!isConvertChanged.at(MatrixConvert::Move))
        return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Move));

    if (objectShift == nullptr)
        throw std::invalid_argument("Couldn't get move convert");

    isConvertChanged.at(MatrixConvert::Move) = false;
    convertCached.at(MatrixConvert::Move) = Matrix<4, 4>::getMoveConvert(*objectShift);
    return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Move));
}

const std::unique_ptr<Matrix<4, 4>> ConvertsCache::getViewConvert()
{
    if (!isConvertChanged.at(MatrixConvert::View))
        return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::View));

    isConvertChanged.at(MatrixConvert::View) = false;
    convertCached.at(MatrixConvert::View) = Matrix<4, 4>::getViewConvert(camera.cGetPosition(), camera.cGetTarget(), up);
    return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::View));
}

const std::unique_ptr<Matrix<4, 4>> ConvertsCache::getProjectionConvert()
{
    if (!isConvertChanged.at(MatrixConvert::Projection))
        return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Projection));

    isConvertChanged.at(MatrixConvert::Projection) = false;
    convertCached.at(MatrixConvert::Projection) = Matrix<4, 4>::getProjectionConvert(camera.getFOV(), camera.getAspect(), 2000, 0.1);
    return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Projection));
}

const std::unique_ptr<Matrix<4, 4>> ConvertsCache::getViewportConvert()
{
    if (!isConvertChanged.at(MatrixConvert::Viewport))
        return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Viewport));

    isConvertChanged.at(MatrixConvert::Viewport) = false;
    convertCached.at(MatrixConvert::Viewport) = Matrix<4, 4>::getViewportConvert(camera.cGetResolution().cGetX(), camera.cGetResolution().cGetY(), 0, 0);
    return std::make_unique<Matrix<4, 4>>(convertCached.at(MatrixConvert::Viewport));
}
