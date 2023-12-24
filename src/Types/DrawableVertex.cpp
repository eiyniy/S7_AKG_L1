#include <DrawableVertex.hpp>

DrawableVertex::DrawableVertex()
    : x(0), y(0), z(0),
      isWNegative(false) {}

DrawableVertex::DrawableVertex(const DrawableVertex &drawableVertex) = default;

DrawableVertex &DrawableVertex::operator=(const DrawableVertex &drawableVertex)
{
  if (this == &drawableVertex)
    return *this;

  x = drawableVertex.x;
  y = drawableVertex.y;
  z = drawableVertex.z;
  isWNegative = drawableVertex.isWNegative;

  return *this;
}

DrawableVertex::DrawableVertex(
    const double _x, const double _y, const double _z,
    const bool _isWNegative)
    : x(_x), y(_y), z(_z),
      isWNegative(_isWNegative) {}

DrawableVertex::DrawableVertex(
    const int _x, const int _y, const double _z,
    const bool _isWNegative)
    : x(_x), y(_y), z(_z),
      isWNegative(_isWNegative) {}