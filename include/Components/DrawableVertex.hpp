#pragma once

class DrawableVertex {
private:
    int x, y;
    double z;
    bool isVisible, isWNegative;

public:
    DrawableVertex();

    DrawableVertex(const DrawableVertex &drawableVertex);

    DrawableVertex &operator=(const DrawableVertex &drawableVertex);

    DrawableVertex(
            int x, int y, double z,
            bool isVisible = true, bool isWNegative = false);

    [[nodiscard]] int CGetX() const;

    [[nodiscard]] int CGetY() const;

    [[nodiscard]] double CGetZ() const;

    [[nodiscard]] int &GetX();

    [[nodiscard]] int &GetY();

    [[nodiscard]] double &GetZ();

    [[nodiscard]] bool IsVisible() const;

    [[nodiscard]] bool IsWNegative() const;
};

inline int &DrawableVertex::GetX() {
    return x;
}

inline int &DrawableVertex::GetY() {
    return y;
}

inline double &DrawableVertex::GetZ() {
    return z;
}

inline int DrawableVertex::CGetX() const {
    return x;
}

inline int DrawableVertex::CGetY() const {
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
