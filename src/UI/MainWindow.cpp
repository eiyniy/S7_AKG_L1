#include <iostream>
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
    sf::Clock clock;
    float dt = 0.f;
    auto moveAxis = AxisName::X;
    auto moveDirection = Direction::Forward;

    scene.convertAllModels();

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
            window.setView(sf::View({0, 0, (float)event.size.width, (float)event.size.height}));
            bufferTexture.create(event.size.width, event.size.height);
            scene.getCamera().getResolution() = Dot(event.size.width, event.size.height);

            delete[] pixels;
            pixels = new sf::Uint8[event.size.width * event.size.height * 4];

            scene.convertAllModels();

            break;
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
            else if (event.key.code == sf::Keyboard::C)
                isCentering = false;

            break;
        }

        if (isCameraMoving)
        {
            auto transition = scene.getMoveConvert(moveAxis, moveDirection, dt);
            scene.moveCamera(transition);
        }
        else if (isCameraRotatingAround)
        {
            scene.rotateCameraAround(moveAxis, moveDirection, dt);
        }
        else if (isObjectMoving)
        {
            auto transition = scene.getMoveConvert(moveAxis, moveDirection, dt);
            scene.getObjectShift(scene.getSelectedObjectName()) = scene.getObjectShift(scene.getSelectedObjectName()) + transition;
        }
        else if (isCentering)
        {
            scene.centralizeCamera();
        }

        if (isCameraMoving || isCameraRotatingAround || isObjectMoving || isCentering)
            scene.convertAllModels();

        drawAllModels();
        window.display();

        dt = clock.restart().asMilliseconds();
    }
}

void MainWindow::drawAllModels()
{
    window.clear();

    auto resolution = scene.cGetCamera().cGetResolution();
    std::fill(pixels, pixels + resolution.x * resolution.y * 4, 0x00u);

    drawModel(
        *scene.getObject(scene.floorObjectName),
        scene.getObjectConvertedVertices(scene.floorObjectName));

    for (auto key : scene.getAllObjectNames())
    {
        if (key == scene.floorObjectName)
            continue;

        drawModel(*scene.getObject(key), scene.getObjectConvertedVertices(key));
    }

    bufferTexture.update(pixels);
    window.draw(bufferSprite);
}

void MainWindow::drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices)
{
    auto color = objInfo.getColor();

    /*
    for (auto vertex : viewportVertices)
    {
        if (vertex.cGetIsOutOfScreen() || vertex.cGetIsWNegative())
            continue;

        int x = std::trunc(vertex.cGetX());
        int y = std::trunc(vertex.cGetY());

        pixels[4 * (y * resolution.x + x)] = 255;     // R
        pixels[4 * (y * resolution.x + x) + 1] = 0;   // G
        pixels[4 * (y * resolution.x + x) + 2] = 0;   // B
        pixels[4 * (y * resolution.x + x) + 3] = 255; // A
    }
    */

    // /*
    for (auto polygon : objInfo.cGetPolygons())
    {
        auto vIndexesCount = polygon.cGetVertexIndexesCount();
        bool isPolygonVisible = false;

        for (int i = 0; i < vIndexesCount; ++i)
            isPolygonVisible |= !viewportVertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1].cGetIsOutOfScreen();

        if (!isPolygonVisible)
            continue;

        auto polygonVertices = std::vector<sf::Vertex>();
        polygonVertices.reserve(vIndexesCount);

        for (int i = 0, j = -1; i < vIndexesCount; ++i)
        {
            auto vertex = viewportVertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1];
            if (vertex.cGetIsWNegative())
                continue;

            polygonVertices.emplace_back(
                sf::Vertex(
                    sf::Vector2f(
                        vertex.cGetX(),
                        vertex.cGetY()),
                    color));
            ++j;

            if (polygonVertices.size() < 2)
                continue;

            auto line = std::array<sf::Vertex, 2>{
                polygonVertices[j - 1],
                polygonVertices[j]};

            window.draw(line.data(), 2, sf::Lines);
        }

        if (!(viewportVertices[polygon.cGetVertexIndexes(0).cGetVertexId() - 1].cGetIsWNegative() &&
              viewportVertices[polygon.cGetVertexIndexes(vIndexesCount - 1).cGetVertexId() - 1].cGetIsWNegative() &&
              polygonVertices.size() > 1))
            continue;

        auto line = std::array<sf::Vertex, 2>{
            *(polygonVertices.cend() - 1),
            *polygonVertices.cbegin()};

        window.draw(line.data(), 2, sf::Lines);
    }
    // */
}
