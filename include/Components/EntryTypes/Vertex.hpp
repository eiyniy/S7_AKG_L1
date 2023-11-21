#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex {
public:
    Vertex();

    Vertex(const Vertex &v);

    Vertex &operator=(const Vertex &v);

    Vertex(double x,
           double y,
           double z,
           double w = 1);

    static Vertex parse(const std::string &line);

    [[nodiscard]] double cGetX() const;

    [[nodiscard]] double cGetY() const;

    [[nodiscard]] double cGetZ() const;

    [[nodiscard]] double cGetW() const;

    double &getX();

    double &getY();

    double &getZ();

    double &getW();
};

inline double Vertex::cGetX() const {
    return v1;
}

inline double Vertex::cGetY() const {
    return v2;
}

inline double Vertex::cGetZ() const {
    return v3;
}

inline double Vertex::cGetW() const {
    return v4;
}

inline double &Vertex::getX() {
    return v1;
}

inline double &Vertex::getY() {
    return v2;
}

inline double &Vertex::getZ() {
    return v3;
}

inline double &Vertex::getW() {
    return v4;
}
