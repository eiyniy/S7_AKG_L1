#include <iostream>
#include <thread>
#include <MainWindow.hpp>
#include <Timer.hpp>

MainWindow::MainWindow(Scene &p_scene)
    : window(sf::RenderWindow(
          sf::VideoMode(
              p_scene.cGetCamera().cGetResolution().x,
              p_scene.cGetCamera().cGetResolution().y),
          "SFML Graphics")),
      scene(p_scene),
      isCameraMoving(false),
      isObjectMoving(false),
      isCameraRotating(false),
      isCameraRotatingAround(false),
      isCentering(false)
{
    window.setFramerateLimit(scene.defaultFps);

    auto resolution = scene.cGetCamera().cGetResolution();

    pixels = new sf::Uint8[resolution.x * resolution.y * 4];
    bufferTexture.create(resolution.x, resolution.y);

    bufferSprite.setTexture(bufferTexture);
}

void MainWindow::startLoop()
{
    scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
    scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());

    sf::Clock clock;
    float dt = 0.f;

    auto moveAxis = AxisName::X;
    auto moveDirection = Direction::Forward;

    sf::Event event;

    while (window.isOpen())
    {
        window.pollEvent(event);

        switch (event.type)
        {
        case sf::Event::Closed:
            window.close();
            break;

        case sf::Event::Resized:
        {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));

            scene.getCamera().getResolution() = Dot(event.size.width, event.size.height);

            delete[] pixels;
            pixels = new sf::Uint8[event.size.width * event.size.height * 4];

            bufferTexture.create(event.size.width, event.size.height);

            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
            scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());

            break;
        }
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Up ||
                event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::Down ||
                event.key.code == sf::Keyboard::Left)
            {
                if (event.key.control && !event.key.alt)
                    isObjectMoving = true;
                else if (!event.key.control && event.key.alt)
                    isCameraRotating = true;
                else if (event.key.control && event.key.alt)
                    isCameraRotatingAround = true;
                else if (!event.key.control && !event.key.alt)
                    isCameraMoving = true;

                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Right)
                    moveDirection = Direction::Forward;
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Left)
                    moveDirection = Direction::Backward;
            }
            else if (event.key.code == sf::Keyboard::X)
                moveAxis = AxisName::X;
            else if (event.key.code == sf::Keyboard::Y)
                moveAxis = AxisName::Y;
            else if (event.key.code == sf::Keyboard::Z)
                moveAxis = AxisName::Z;
            else if (event.key.code == sf::Keyboard::C && event.key.control)
                isCentering = true;

            break;

        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Up ||
                event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::Down ||
                event.key.code == sf::Keyboard::Left)
            {
                isCameraMoving = false;
                isCameraRotating = false;
                isCameraRotatingAround = false;
                isObjectMoving = false;
            }

            break;

        default:
            break;
        }

        if (isCameraMoving)
        {
            auto transition = scene.getMoveConvert(moveAxis, moveDirection, dt);
            scene.moveCamera(transition);
            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
            scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());
        }
        else if (isCameraRotating || isCameraRotatingAround)
        {
            if (isCameraRotating)
            {
                // scene.rotateCamera(moveAxis, angle);
                scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
                scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());
            }
            else if (isCameraRotatingAround)
            {
                scene.rotateCameraAround(moveAxis, moveDirection, dt);
                scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
                scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());
            }
        }
        else if (isObjectMoving)
        {
            auto transition = scene.getMoveConvert(moveAxis, moveDirection, dt);

            scene.getWorldShift() = scene.getWorldShift() + transition;
            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
            scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());
        }
        else if (isCentering)
        {
            isCentering = false;
            scene.centralizeCamera();
            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.getObjInfoVertices(), scene.cGetWorldShift());
            scene.modelConvert(scene.cGetFloorVerticesCopy(), scene.getFloorVertices());
        }

        draw();
        dt = clock.restart().asMilliseconds();
    }
}

void MainWindow::draw()
{
    window.clear();

    auto resolution = scene.cGetCamera().cGetResolution();

    std::fill(pixels, pixels + resolution.x * resolution.y * 4, 0x00u);

    auto vertices2 = scene.cGetFloorVertices();

    // /*
    for (auto el : vertices2)
    {
        if (el.cGetIsOutOfScreen() || el.cGetIsWNegative())
            continue;

        int x = std::trunc(el.cGetX());
        int y = std::trunc(el.cGetY());

        pixels[4 * (y * resolution.x + x)] = 255;     // R
        pixels[4 * (y * resolution.x + x) + 1] = 0;   // G
        pixels[4 * (y * resolution.x + x) + 2] = 0;   // B
        pixels[4 * (y * resolution.x + x) + 3] = 255; // A
    }
    // */

    // /*
    auto drawVertices2 = std::vector<sf::Vertex>();

    for (auto polygon : scene.cGetFloor().cGetPolygons())
    {
        auto vIndexesCount = polygon.cGetVertexIndexesCount();
        bool isPolygonVisible = false;

        for (int i = 0; i < vIndexesCount; ++i)
            isPolygonVisible |= !vertices2[polygon.cGetVertexIndexes(i).cGetVertexId() - 1].cGetIsOutOfScreen();

        if (!isPolygonVisible)
            continue;

        drawVertices2.clear();
        drawVertices2.reserve(vIndexesCount);

        for (int i = 0, j = -1; i < vIndexesCount; ++i)
        {
            auto vertex = vertices2[polygon.cGetVertexIndexes(i).cGetVertexId() - 1];
            if (vertex.cGetIsWNegative())
                continue;

            drawVertices2.emplace_back(sf::Vertex(sf::Vector2f(vertex.cGetX(), vertex.cGetY()), sf::Color::Blue));
            ++j;

            if (drawVertices2.size() < 2)
                continue;

            auto line = std::array<sf::Vertex, 2>{
                drawVertices2[j - 1],
                drawVertices2[j]};

            window.draw(line.data(), 2, sf::Lines);
        }

        if (!(vertices2[polygon.cGetVertexIndexes(0).cGetVertexId() - 1].cGetIsWNegative() &&
              vertices2[polygon.cGetVertexIndexes(vIndexesCount - 1).cGetVertexId() - 1].cGetIsWNegative() &&
              drawVertices2.size() > 1))
            continue;

        auto line = std::array<sf::Vertex, 2>{
            *(drawVertices2.cend() - 1),
            *drawVertices2.cbegin()};

        window.draw(line.data(), 2, sf::Lines);
    }
    // */

    auto vertices = scene.cGetObjInfoVertices();

    /*
    for (auto el : vertices)
    {
        if (el.cGetIsOutOfScreen() || el.cGetIsWNegative())
            continue;

        int x = std::trunc(el.cGetX());
        int y = std::trunc(el.cGetY());

        pixels[4 * (y * resolution.x + x)] = 255;     // R
        pixels[4 * (y * resolution.x + x) + 1] = 255; // G
        pixels[4 * (y * resolution.x + x) + 2] = 0;   // B
        pixels[4 * (y * resolution.x + x) + 3] = 255; // A
    }
    */

    auto drawVertices = std::vector<sf::Vertex>();

    for (auto polygon : scene.cGetObjInfo().cGetPolygons())
    {
        auto vIndexesCount = polygon.cGetVertexIndexesCount();
        bool isPolygonVisible = false;

        for (int i = 0; i < vIndexesCount; ++i)
            isPolygonVisible |= !vertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1].cGetIsOutOfScreen();

        if (!isPolygonVisible)
            continue;

        drawVertices.clear();
        drawVertices.reserve(vIndexesCount);

        for (int i = 0, j = -1; i < vIndexesCount; ++i)
        {
            auto vertex = vertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1];
            if (vertex.cGetIsWNegative())
                continue;

            drawVertices.emplace_back(sf::Vertex(sf::Vector2f(vertex.cGetX(), vertex.cGetY())));
            ++j;

            if (drawVertices.size() < 2)
                continue;

            auto line = std::array<sf::Vertex, 2>{
                drawVertices[j - 1],
                drawVertices[j]};

            window.draw(line.data(), 2, sf::Lines);
        }

        if (!(vertices[polygon.cGetVertexIndexes(0).cGetVertexId() - 1].cGetIsWNegative() &&
              vertices[polygon.cGetVertexIndexes(vIndexesCount - 1).cGetVertexId() - 1].cGetIsWNegative() &&
              drawVertices.size() > 1))
            continue;

        auto line = std::array<sf::Vertex, 2>{
            *(drawVertices.cend() - 1),
            *drawVertices.cbegin()};

        window.draw(line.data(), 2, sf::Lines);
    }

    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();
}
