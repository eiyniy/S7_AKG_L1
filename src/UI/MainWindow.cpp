#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>
#include <Converter.hpp>

MainWindow::MainWindow(Point &p_resolution)
    : window(sf::RenderWindow(
          sf::VideoMode(p_resolution.cGetX(), p_resolution.cGetY()),
          "SFML Graphics",
          sf::Style::Fullscreen)),
      isFullscreen(true),
      resolution(p_resolution)
{
    pixels = new sf::Uint8[resolution.cGetX() * resolution.cGetY() * 4];
    bufferTexture.create(resolution.cGetX(), resolution.cGetY());
    bufferSprite.setTexture(bufferTexture, true);
}

void MainWindow::drawModel(const ObjInfo &objInfo, const std::vector<Vertex> &viewportVertices)
{
    auto color = &objInfo.getColor();

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

            drawLineBR(
                polygonVertices[i - 1],
                polygonVertices[i],
                color);
        }

        drawLineBR(
            *(polygonVertices.cend() - 1),
            *polygonVertices.cbegin(),
            color);
    }
    // */
}

void MainWindow::switchVideoMode(const bool isEscape)
{
    if (!isFullscreen && isEscape)
        return;

    sf::VideoMode videoMode;

    if (isFullscreen)
        videoMode = sf::VideoMode(1280, 720);
    else
        videoMode = sf::VideoMode::getDesktopMode();

    isFullscreen = !isFullscreen;

    window.create(
        videoMode,
        "SFML Graphics",
        isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

    resize(videoMode.width, videoMode.height);
}

void MainWindow::resize(const int width, const int height)
{
    resolution = Point(width, height);

    delete[] pixels;
    pixels = new sf::Uint8[width * height * 4];

    bufferTexture.create(width, height);
    bufferSprite.setTexture(bufferTexture, true);

    auto view = sf::View(sf::FloatRect(0, 0, width, height));
    window.setView(view);
}

void MainWindow::clear()
{
    window.clear();
    std::fill(pixels, pixels + resolution.cGetX() * resolution.cGetY() * 4, 0x53u);
}

void MainWindow::drawPixels()
{
    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();
}

void MainWindow::drawLineBR(
    const Vertex &v1,
    const Vertex &v2,
    const sf::Color *color)
{
    int x1 = v1.cGetX();
    int y1 = v1.cGetY();

    const int x2 = v2.cGetX();
    const int y2 = v2.cGetY();
    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);
    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    if (x2 < resolution.cGetX() && y2 < resolution.cGetY() && x2 > 0 && y2 > 0)
        drawPixel(x2, y2, color, resolution.cGetX());

    // Timer::start();

    while (x1 != x2 || y1 != y2)
    {
        if (x1 < resolution.cGetX() && y1 < resolution.cGetY() && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.cGetX());

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
    const Vertex &v1,
    const Vertex &v2,
    const sf::Color *color)
{
    const int x2 = v2.cGetX();
    const int y2 = v2.cGetY();

    int x1 = v1.cGetX();
    int y1 = v1.cGetY();

    int deltaX = abs(x1 - x2);
    int deltaY = abs(y1 - y2);

    int length = std::max(deltaX, deltaY);

    if (length == 0)
    {
        if (x1 < resolution.cGetX() && y1 < resolution.cGetY() && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.cGetX());

        return;
    }

    const int dX = (x2 - x1) / length;
    const int dY = (y2 - y1) / length;

    length++;
    while (length--)
    {
        if (x1 < resolution.cGetX() && y1 < resolution.cGetY() && x1 > 0 && y1 > 0)
            drawPixel(x1, y1, color, resolution.cGetX());

        x1 += dX;
        y1 += dY;
    }
}
