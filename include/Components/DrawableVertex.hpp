#pragma once

class DrawableVertex {
private:
    int x, y;
    bool isVisible, isWNegative;

public:
    DrawableVertex();

    DrawableVertex(const DrawableVertex &drawableVertex);

    DrawableVertex &operator=(const DrawableVertex &drawableVertex);

    DrawableVertex(int x, int y, bool isVisible = true, bool isWNegative = false);

    [[nodiscard]] int CGetX() const;

    [[nodiscard]] int CGetY() const;

    [[nodiscard]] int &GetX();

    [[nodiscard]] int &GetY();

    [[nodiscard]] bool IsVisible() const;

    [[nodiscard]] bool IsWNegative() const;
};

inline int &DrawableVertex::GetX() {
    return x;
}

inline int &DrawableVertex::GetY() {
    return y;
}

inline int DrawableVertex::CGetX() const {
    return x;
}

inline int DrawableVertex::CGetY() const {
    return y;
}

inline bool DrawableVertex::IsVisible() const {
    return isVisible;
}

inline bool DrawableVertex::IsWNegative() const {
    return isWNegative;
}
