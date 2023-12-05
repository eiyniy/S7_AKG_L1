#include <DrawableVertex.hpp>

DrawableVertex::DrawableVertex()
        : x(0), y(0),
          isVisible(true),
          isWNegative(false) {}

DrawableVertex::DrawableVertex(const DrawableVertex &drawableVertex)
        : x(drawableVertex.x),
          y(drawableVertex.y),
          z(drawableVertex.z),
          isVisible(drawableVertex.isVisible),
          isWNegative(drawableVertex.isWNegative) {}

DrawableVertex &DrawableVertex::operator=(const DrawableVertex &drawableVertex) {
    if (this == &drawableVertex)
        return *this;

    x = drawableVertex.x;
    y = drawableVertex.y;
    z = drawableVertex.z;
    isVisible = drawableVertex.isVisible;
    isWNegative = drawableVertex.isWNegative;

    return *this;
}

DrawableVertex::DrawableVertex(
        const int x, const int y, const double z,
        const bool isVisible, const bool isWNegative)
        : x(x), y(y), z(z),
          isVisible(isVisible),
          isWNegative(isWNegative) {}
