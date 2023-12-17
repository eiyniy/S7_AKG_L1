#pragma once

#include <BaseVertex.hpp>
#include <string>

class NormalVertex : public BaseVertex {
public:
    NormalVertex(double i, double j, double k);

    static NormalVertex parse(const std::string &line);

    [[nodiscard]] double cGetI() const;

    [[nodiscard]] double cGetJ() const;

    [[nodiscard]] double cGetK() const;
};

inline double NormalVertex::cGetI() const {
    return v1;
}

inline double NormalVertex::cGetJ() const {
    return v2;
}

inline double NormalVertex::cGetK() const {
    return v3;
}
