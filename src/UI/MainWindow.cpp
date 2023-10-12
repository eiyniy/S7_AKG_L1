#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>
#include <Converter.hpp>

MainWindow::MainWindow(Scene &p_scene)
    : window(sf::RenderWindow(
          sf::VideoMode(
              p_scene.cGetCamera().cGetResolution().x,
              p_scene.cGetCamera().cGetResolution().y),
          "SFML Graphics",
          sf::Style::Fullscreen)),
      scene(p_scene),
      isCameraMoving(false),
      isObjectMoving(false),
      isCameraRotating(false),
      isCameraRotatingAround(false),
      isCentering(false),
      isFullscreen(true)
{
    window.setFramerateLimit(scene.defaultFps);

    auto resolution = scene.cGetCamera().cGetResolution();

    pixels = new sf::Uint8[resolution.x * resolution.y * 4];
    bufferTexture.create(resolution.x, resolution.y);
    bufferSprite.setTexture(bufferTexture, true);
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
        {
            resize(event.size.width, event.size.height);
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
            else if (event.key.code == sf::Keyboard::F11)
                resize();

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
            auto transition = scene.getTransition(moveAxis, moveDirection, dt);
            scene.moveCamera(transition);
        }
        else if (isCameraRotatingAround)
        {
            scene.rotateCameraAround(moveAxis, moveDirection, dt);
        }
        else if (isObjectMoving)
        {
            auto transition = scene.getTransition(moveAxis, moveDirection, dt);
            scene.moveObject(scene.getSelectedObjectName(), transition);
        }
        else if (isCentering)
        {
            scene.centralizeCamera();
        }

        if (isCameraMoving || isCameraRotatingAround || isObjectMoving || isCentering)
            scene.convertAllModels();

        drawAllModels();

        dt = clock.restart().asMilliseconds();
    }
}

void MainWindow::drawAllModels()
{
    window.clear();

    auto resolution = scene.cGetCamera().cGetResolution();
    std::fill(pixels, pixels + resolution.x * resolution.y * 4, 0x53u);

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
    window.display();
}

void MainWindow::drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices)
{
    auto color = &objInfo.getColor();
    const Dot resolution = scene.cGetCamera().cGetResolution();

    /*
    for (auto vertex : viewportVertices)
    {
        if (vertex.cGetIsOutOfScreen() || vertex.cGetIsWNegative())
            continue;

        int x = std::trunc(vertex.cGetX());
        int y = std::trunc(vertex.cGetY());

        drawPixel(x, y, color, resolution);
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

        auto polygonVertices = std::vector<Vertex>(vIndexesCount);

        for (int i = 0; i < vIndexesCount; ++i)
        {
            auto vertex = viewportVertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1];
            polygonVertices[i] = vertex;

            if (i < 1)
                continue;

            drawLineBR_1(
                pixels,
                polygonVertices[i - 1],
                polygonVertices[i],
                color);
        }

        drawLineBR_1(
            pixels,
            *(polygonVertices.cend() - 1),
            *polygonVertices.cbegin(),
            color);
    }
    // */
}

void MainWindow::resize()
{
    if (isFullscreen)
    {
        window.create(
            sf::VideoMode(1280, 720),
            "SFML Graphics",
            sf::Style::Default);
        resize(1280, 720);
        isFullscreen = false;
    }
    else
    {
        window.create(
            sf::VideoMode::getDesktopMode(),
            "SFML Graphics",
            sf::Style::Fullscreen);
        resize(1920, 1080);
        isFullscreen = true;
    }
}

void MainWindow::resize(const int width, const int height)
{
    delete[] pixels;
    pixels = new sf::Uint8[width * height * 4];

    bufferTexture.create(width, height);
    bufferSprite.setTexture(bufferTexture, true);

    auto view = sf::View(sf::FloatRect(0, 0, width, height));
    window.setView(view);

    scene.resize(width, height);
}

void MainWindow::drawLineBR_1(
    sf::Uint8 *pixels,
    const Vertex &v1,
    const Vertex &v2,
    const sf::Color *color)
{
    int x1 = v1.cGetX();
    int y1 = v1.cGetY();

    const Dot resolution = scene.cGetCamera().cGetResolution();
    const int x2 = v2.cGetX();
    const int y2 = v2.cGetY();
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    if (x2 < resolution.x && y2 < resolution.y && x2 > 0 && y2 > 0)
        drawPixel(x2, y2, color, resolution.x);

    // Timer::start();

    while (x1 != x2 || y1 != y2)
    {
        if (x1 < resolution.x && y1 < resolution.y && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.x);

        const int error2 = error * 2;
        if (error2 > -deltaY)
        {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX)
        {
            error += deltaX;
            y1 += signY;
        }
    }

    // Timer::stop();
}

void MainWindow::drawLineDDA(
    sf::Uint8 *pixels,
    const Vertex &v1,
    const Vertex &v2,
    const sf::Color *color)
{
    const Dot resolution = scene.cGetCamera().cGetResolution();
    const int x2 = v2.cGetX();
    const int y2 = v2.cGetY();

    int x1 = v1.cGetX();
    int y1 = v1.cGetY();

    int deltaX = abs(x1 - x2);
    int deltaY = abs(y1 - y2);

    int length = std::max(deltaX, deltaY);

    if (length == 0)
    {
        if (x1 < resolution.x && y1 < resolution.y && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.x);

        return;
    }

    const int dX = (x2 - x1) / length;
    const int dY = (y2 - y1) / length;

    length++;
    while (length--)
    {
        if (x1 < resolution.x && y1 < resolution.y && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.x);

        x1 += dX;
        y1 += dY;
    }
}
