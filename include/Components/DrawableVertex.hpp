#pragma once

class DrawableVertex {
private:
    double x, y;
    double z;
    bool isVisible, isWNegative;

public:
    DrawableVertex();

    DrawableVertex(const DrawableVertex &drawableVertex);

    DrawableVertex &operator=(const DrawableVertex &drawableVertex);

    DrawableVertex(
            double _x, double _y, double _z,
            bool _isVisible = true, bool _isWNegative = false);

    DrawableVertex(
            int _x, int _y, double _z,
            bool _isVisible = true, bool _isWNegative = false);

    [[nodiscard]] double CGetX() const;

    [[nodiscard]] double CGetY() const;

    [[nodiscard]] double CGetZ() const;

    [[nodiscard]] double &GetX();

    [[nodiscard]] double &GetY();

    [[nodiscard]] double &GetZ();

    [[nodiscard]] bool IsVisible() const;

    [[nodiscard]] bool IsWNegative() const;
};

inline double &DrawableVertex::GetX() {
    return x;
}

inline double &DrawableVertex::GetY() {
    return y;
}

inline double &DrawableVertex::GetZ() {
    return z;
}

inline double DrawableVertex::CGetX() const {
    return x;
}

inline double DrawableVertex::CGetY() const {
    return y;
}

inline double DrawableVertex::CGetZ() const {
    return z;
}

inline bool DrawableVertex::IsVisible() const {
    return isVisible;
}

inline bool DrawableVertex::IsWNegative() const {
    return isWNegative;
}
