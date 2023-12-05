#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>
#include <Converter.hpp>
#include <ThreadPool.hpp>
#include <SHClipper.hpp>
#include <BarycentricRasterizer.hpp>

MainWindow::MainWindow(Point &_resolution)
        : window(sf::RenderWindow(
        sf::VideoMode(_resolution.cGetX(), _resolution.cGetY()),
        "SFML Graphics",
        sf::Style::Fullscreen)),
          isFullscreen(true),
          resolution(_resolution),
          lastResolution(Point(1280, 720)),
          clipper(std::make_unique<CSClipper>(resolution.cGetX() - 1, resolution.cGetY() - 1, 0, 0)) {
    pixels = new sf::Uint8[resolution.cGetX() * resolution.cGetY() * 4];

    depthBuffer = new double[resolution.cGetX() * resolution.cGetY()];
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);

    bufferTexture.create(resolution.cGetX(), resolution.cGetY());
    bufferSprite.setTexture(bufferTexture, true);
}

void MainWindow::drawModel(Object &objInfo, std::vector<DrawableVertex> &viewportVertices) {
    colorNumber = 0;

    const auto color = &objInfo.cGetColor();

    auto polygons = objInfo.getPolygons();

    /*
   // const int threadsCount = (unsigned int)ceil(polygons.size() / 10000.f);
   const auto threadsCount = std::min(
           (unsigned int) ceil(polygons.size() / 10000.f),
           ThreadPool::getInstance().getThreadsCount());
   const double size = polygons.size() / (double) threadsCount;

   for (int i = 0; i < threadsCount; ++i) {
       const int begin = floor(size * i);
       const int end = floor(size * (i + 1)) - 1;

       ThreadPool::getInstance().enqueue(
               [this, begin, end, &polygons, &viewportVertices, color]() {
                   for (int j = begin; j <= end; ++j) {
                       drawPolygon(polygons[j], viewportVertices, color);
                   }
               });
   }

   ThreadPool::getInstance().waitAll();
    */

    for (int j = 0; j <= polygons.size(); ++j) {
        drawPolygon(polygons[j], viewportVertices, color);
    }

    /*
for (auto vertex: viewportVertices) {
    if (!vertex.IsVisible())
        continue;

    int x = vertex.CGetX();
    int y = vertex.CGetY();

//        if (x - 1 < 0 || x + 1 >= resolution.cGetX() || y - 1 < 0 || y + 1 >= resolution.cGetY())
//            continue;

    drawPixel(x - 1, y - 1, &vColor, resolution.cGetX());
    drawPixel(x - 1, y, &vColor, resolution.cGetX());
    drawPixel(x - 1, y + 1, &vColor, resolution.cGetX());
    drawPixel(x, y - 1, &vColor, resolution.cGetX());
    drawPixel(x, y, &vColor, resolution.cGetX());
    drawPixel(x, y + 1, &vColor, resolution.cGetX());
    drawPixel(x + 1, y - 1, &vColor, resolution.cGetX());
    drawPixel(x + 1, y, &vColor, resolution.cGetX());
    drawPixel(x + 1, y + 1, &vColor, resolution.cGetX());
}
*/

    /*
    for (int j = 0; j < polygons.size(); ++j)
        drawPolygon(polygons[j], viewportVertices, color);
    */
}

void MainWindow::switchVideoMode(const bool isEscape) {
    if (!isFullscreen && isEscape)
        return;

    sf::VideoMode videoMode;

    if (isFullscreen)
        videoMode = sf::VideoMode(lastResolution.cGetX(), lastResolution.cGetY());
    else {
        lastResolution = Point(window.getSize().x, window.getSize().y);
        videoMode = sf::VideoMode::getDesktopMode();
    }

    isFullscreen = !isFullscreen;

    window.create(
            videoMode,
            "SFML Graphics",
            isFullscreen ? sf::Style::Fullscreen : sf::Style::Default);

    resize(videoMode.width, videoMode.height);
}

void MainWindow::resize(const int width, const int height) {
    resolution = Point(width, height);

    delete[] pixels;
    pixels = new sf::Uint8[width * height * 4];

    depthBuffer = new double[width * height];
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);


    bufferTexture.create(width, height);
    bufferSprite.setTexture(bufferTexture, true);

    clipper = std::make_unique<CSClipper>(resolution.cGetX() - 1, resolution.cGetY() - 1, 0, 0);

    auto view = sf::View(sf::FloatRect(0, 0, width, height));
    window.setView(view);
}

void MainWindow::clear() {
    window.clear();
    std::fill(pixels, pixels + resolution.cGetX() * resolution.cGetY() * 4, 0x53u);
    std::fill(depthBuffer, depthBuffer + resolution.cGetX() * resolution.cGetY(), INT_MAX);
}

void MainWindow::drawPixels() {
    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();
}

void MainWindow::drawPolygon(
        const Polygon &polygon,
        std::vector<DrawableVertex> &drawableVertices,
        const sf::Color *color) {
    if (colors.empty()) {
        for (int i = 0; i < 10; ++i)
            colors.emplace_back(rand() % 255, rand() % 255, rand() % 255);
    }

    auto vIndexesCount = polygon.cGetVertexIdsCount();
//    /*
    bool isPolygonVisible = false;

    for (int i = 0; i < vIndexesCount; ++i)
        isPolygonVisible |= drawableVertices[polygon.cGetVertexIds(i).cGetVertexId() - 1].IsVisible();

    if (!isPolygonVisible)
        return;
//    */

    /*
    for (int i = 0; i < vIndexesCount; ++i) {
        const auto j = (i + 1) % vIndexesCount;

        auto firstDrawablePoint = drawableVertices[polygon.cGetVertexIds(i).cGetVertexId() - 1];
        auto secondDrawablePoint = drawableVertices[polygon.cGetVertexIds(j).cGetVertexId() - 1];

        if (firstDrawablePoint.IsWNegative() || secondDrawablePoint.IsWNegative())
            continue;

        const auto result = clipper->clipLine(
                firstDrawablePoint.GetX(),
                firstDrawablePoint.GetY(),
                secondDrawablePoint.GetX(),
                secondDrawablePoint.GetY());

        if (result == ClipLineResult::Invisible)
            continue;

        drawLineBr(
                firstDrawablePoint,
                secondDrawablePoint,
                color);
    }
    */

//    /*
    BarycentricRasterizer::rasterize(
            polygon,
            drawableVertices,
            depthBuffer,
            resolution.cGetX(),
            pixels,
            &colors.at(colorNumber));
//    for (const auto &i: result) {
//        drawPixel(i.CGetX(), i.CGetY(), &colors.at(colorNumber), resolution.cGetX());
//    }

    colorNumber = (colorNumber + 1) % colors.size();
//    */
}

void MainWindow::drawLineBr(
        const DrawableVertex &p1,
        const DrawableVertex &p2,
        const sf::Color *color) {
    int x1 = p1.CGetX();
    int y1 = p1.CGetY();

    const int x2 = p2.CGetX();
    const int y2 = p2.CGetY();

    const int xSize = resolution.cGetX();

    const int deltaX = abs(x2 - x1);
    const int deltaY = abs(y2 - y1);

    const int signX = x1 < x2 ? 1 : -1;
    const int signY = y1 < y2 ? 1 : -1;

    int error = deltaX - deltaY;

    // if (x2 < resolution.cGetX() && y2 < resolution.cGetY() && x2 > 0 && y2 > 0)
    drawPixel(x2, y2, color, xSize);

    while (x1 != x2 || y1 != y2) {
        // if (x1 < resolution.cGetX() && y1 < resolution.cGetY() && x1 > 0 && y1 > 0)
        drawPixel(x1, y1, color, xSize);

        const int error2 = error * 2;
        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}
