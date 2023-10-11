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
            scene.getCamera().getResolution() = Dot(event.size.width, event.size.height);

            delete[] pixels;
            pixels = new sf::Uint8[event.size.width * event.size.height * 4];

            bufferTexture.create(event.size.width, event.size.height);
            bufferSprite.setTexture(bufferTexture, true);

            auto view = sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height));
            window.setView(view);

            scene.convertAllModels();

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

void MainWindow::drawLineBR_1(
    sf::Uint8 *pixels,
    const Dot p1,
    const Dot p2,
    const sf::Color color)
{
    int x1 = p1.x;
    int y1 = p1.y;

    const Dot resolution = scene.cGetCamera().cGetResolution();
    const int deltaX = abs(p2.x - x1);
    const int deltaY = abs(p2.y - y1);
    const int signX = x1 < p2.x ? 1 : -1;
    const int signY = y1 < p2.y ? 1 : -1;

    int error = deltaX - deltaY;

    if (p2.x < resolution.x && p2.y < resolution.y && p2.x > 0 && p2.y > 0)
        drawPixel(p2.x, p2.y, color, resolution);

    while (x1 != p2.x || y1 != p2.y)
    {
        if (x1 < resolution.x && y1 < resolution.y && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution);

        int error2 = error * 2;
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
}

void MainWindow::drawLineDDA(
    sf::Uint8 *pixels,
    const Dot p1,
    const Dot p2,
    const sf::Color color)
{
    const Dot resolution = scene.cGetCamera().cGetResolution();

    int deltaX = abs(p1.x - p2.x);
    int deltaY = abs(p1.y - p2.y);

    int length = std::max(deltaX, deltaY);

    if (length == 0)
    {
        if (p1.x < resolution.x && p1.y < resolution.y && p1.x > 0 && p1.y > 0)
            drawPixel(p1.x, p1.y, color, resolution);

        return;
    }

    const int dX = (p2.x - p1.x) / length;
    const int dY = (p2.y - p1.y) / length;

    int x = p1.x;
    int y = p1.y;

    length++;
    while (length--)
    {
        if (x < resolution.x && y < resolution.y && x > 0 && y > 0)
            drawPixel(x, y, color, resolution);

        x += dX;
        y += dY;
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
}

void MainWindow::drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices)
{
    auto color = objInfo.getColor();
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
        // bool isPolygonVisible = true;

        for (int i = 0; i < vIndexesCount; ++i)
            isPolygonVisible |= !viewportVertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1].cGetIsOutOfScreen();
        // isPolygonVisible &= !viewportVertices[polygon.cGetVertexIndexes(i).cGetVertexId() - 1].cGetIsOutOfScreen();

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

            // auto line = std::array<sf::Vertex, 2>{
            //     polygonVertices[j - 1],
            //     polygonVertices[j]};
            // window.draw(line.data(), 2, sf::Lines);

            drawLineBR_1(
                pixels,
                Dot(polygonVertices[j - 1].position.x,
                    polygonVertices[j - 1].position.y),
                Dot(polygonVertices[j].position.x,
                    polygonVertices[j].position.y),
                color);
        }

        if (!(viewportVertices[polygon.cGetVertexIndexes(0).cGetVertexId() - 1].cGetIsWNegative() &&
              viewportVertices[polygon.cGetVertexIndexes(vIndexesCount - 1).cGetVertexId() - 1].cGetIsWNegative() &&
              polygonVertices.size() > 1))
            continue;

        // auto line = std::array<sf::Vertex, 2>{
        //     *(polygonVertices.cend() - 1),
        //     *polygonVertices.cbegin()};
        // window.draw(line.data(), 2, sf::Lines);

        drawLineBR_1(
            pixels,
            Dot((polygonVertices.cend() - 1)->position.x,
                (polygonVertices.cend() - 1)->position.y),
            Dot(polygonVertices.cbegin()->position.x,
                polygonVertices.cbegin()->position.y),
            color);
    }
    // */
}

void MainWindow::drawPixel(
    const int x,
    const int y,
    sf::Color color,
    const Dot resolution)
{
    pixels[4 * (y * resolution.x + x)] = color.r;     // R
    pixels[4 * (y * resolution.x + x) + 1] = color.g; // G
    pixels[4 * (y * resolution.x + x) + 2] = color.b; // B
    pixels[4 * (y * resolution.x + x) + 3] = color.a; // A
}
