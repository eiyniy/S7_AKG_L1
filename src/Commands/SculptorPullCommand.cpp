#include <SculptorPullCommand.hpp>
#include <iostream>

SculptorPullCommand::SculptorPullCommand(
    Sculptor &_sculptor,
    Object *_object,
    const MainWindow &_mainWindow,
    const Point &_mousePos,
    const Vector<4> &_direction)
    : sculptor(_sculptor),
      object(_object),
      mainWindow(_mainWindow),
      mousePos(_mousePos),
      direction(_direction) {}

void SculptorPullCommand::execute()
{
    // std::cout << "SculptorPullCommand log:" << std::endl;
    // direction.log();

    sculptor.createGraph(object);
    sculptor.pull(
        object->getVertices(),
        mousePos,
        direction);

    /*
    const auto pointedId = mainWindow.getPointedTriangleId(object, mousePos);
    sculptor.pull(object, pointedId, mousePos);
    */
}