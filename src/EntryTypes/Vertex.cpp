#include <Vertex.hpp>
#include <ObjParser.hpp>

Vertex::Vertex()
    : BaseVertex(),
      isOutOfScreen(false),
      isWNegative(false) {}

Vertex::Vertex(const Vertex &v)
    : BaseVertex(v),
      isOutOfScreen(v.isOutOfScreen),
      isWNegative(v.isWNegative) {}

Vertex &Vertex::operator=(const Vertex &v)
{
    if (this == &v)
        return *this;

    *this = Vertex(v);

    return *this;
}

Vertex::Vertex(const double x,
               const double y,
               const double z,
               const double w,
               const bool p_isOutOfScreen,
               const bool p_isWNegative)
    : BaseVertex(x, y, z, w),
      isOutOfScreen(p_isOutOfScreen),
      isWNegative(p_isWNegative) {}

Vertex Vertex::parse(const std::string &line)
{
    auto acc = BaseVertex::parse(line);
    return Vertex(acc[0], acc[1], acc[2], acc[3]);
}

const bool Vertex::cGetIsOutOfScreen() const
{
    return isOutOfScreen;
}

const bool Vertex::cGetIsWNegative() const
{
    return isWNegative;
}

const double Vertex::cGetX() const
{
    return v1;
}

const double Vertex::cGetY() const
{
    return v2;
}

const double Vertex::cGetZ() const
{
    return v3;
}

const double Vertex::cGetW() const
{
    return v4;
}

double &Vertex::getX()
{
    return v1;
}

double &Vertex::getY()
{
    return v2;
}

double &Vertex::getZ()
{
    return v3;
}

double &Vertex::getW()
{
    return v4;
}
