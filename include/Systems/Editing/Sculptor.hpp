#pragma once

#include <memory>
#include <Object.hpp>
#include <Point.hpp>
#include <Graph.hpp>
// #include <graph.hpp>

class Sculptor
{
private:
    int radius;
    Graph graph;

    sf::CircleShape circle;

public:
    Sculptor(const int _radius);

    inline int cGetRadius() const;
    inline sf::CircleShape &getCircle();

    void createGraph(Object *object);

    void pull(
        std::vector<Vector<4>> &vertices,
        const Point &mousePos,
        const Vector<4> &direction);

    void pull(
        Object *object,
        const int triangleId,
        const Point mousePos);
};

inline int Sculptor::cGetRadius() const
{
    return radius;
}

inline sf::CircleShape &Sculptor::getCircle()
{
    return circle;
}
