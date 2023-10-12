#pragma once

#include <BaseVertex.hpp>
#include <vector>
#include <string>
#include <optional>

class Vertex : public BaseVertex
{
public:
    Vertex();
    Vertex(const Vertex &v);

    Vertex &operator=(const Vertex &v);

    Vertex(const double x,
           const double y,
           const double z,
           const double w = 1,
           const bool p_isOutOfScreen = false);

    static Vertex parse(const std::string &line);

    const bool cGetIsOutOfScreen() const;

    const double cGetX() const;
    const double cGetY() const;
    const double cGetZ() const;
    const double cGetW() const;

    double &getX();
    double &getY();
    double &getZ();
    double &getW();

private:
    bool isOutOfScreen;
};

inline const double Vertex::cGetX() const
{
    return v1;
}

inline const double Vertex::cGetY() const
{
    return v2;
}

inline const double Vertex::cGetZ() const
{
    return v3;
}

inline const double Vertex::cGetW() const
{
    return v4;
}

inline double &Vertex::getX()
{
    return v1;
}

inline double &Vertex::getY()
{
    return v2;
}

inline double &Vertex::getZ()
{
    return v3;
}

inline double &Vertex::getW()
{
    return v4;
}
