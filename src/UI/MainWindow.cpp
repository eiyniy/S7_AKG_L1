#include <iostream>
#include <thread>
#include <MainWindow.hpp>
#include <Timer.hpp>

MainWindow::MainWindow(Scene &p_scene)
    : window({sf::VideoMode(p_scene.cGetCamera().cGetResolution().x,
                            p_scene.cGetCamera().cGetResolution().y),
              "SFML Graphics"}),
      scene(p_scene),
      isDrawed(false),
      isMoving(false),
      isXPressed(false),
      isYPressed(false),
      isZPressed(false)
{
    window.setFramerateLimit(scene.defaultFps);
    pixels = new sf::Uint8[scene.cGetCamera().cGetResolution().x * scene.cGetCamera().cGetResolution().y * 4];
    bufferTexture.create(scene.cGetCamera().cGetResolution().x, scene.cGetCamera().cGetResolution().y);
    bufferSprite.setTexture(bufferTexture);
    // buffer.create(scene.cGetCamera().cGetResolution().x, scene.cGetCamera().cGetResolution().y, sf::Color::Black);
}

void MainWindow::startLoop()
{
    scene.modelConvert();

    sf::Clock clock;
    float dt = 0.f;

    while (window.isOpen())
    {
        auto moveAxis = AxisName::X;
        auto moveDirection = Direction::Forward;

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

            case sf::Event::Resized:
                scene.getCamera().getResolution() = Dot(event.size.width, event.size.height);
                scene.modelConvert();
                isDrawed = false;
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Up ||
                    event.key.code == sf::Keyboard::Right ||
                    event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::Left)
                {
                    if (isXPressed || isYPressed || isZPressed)
                    {
                        isMoving = true;
                        isDrawed = false;
                    }

                    if (isXPressed)
                        moveAxis = AxisName::X;
                    else if (isYPressed)
                        moveAxis = AxisName::Y;
                    else if (isZPressed)
                        moveAxis = AxisName::Z;
                }

                if (event.key.code == sf::Keyboard::X)
                    isXPressed = true;
                else if (event.key.code == sf::Keyboard::Y)
                    isYPressed = true;
                else if (event.key.code == sf::Keyboard::Z)
                    isZPressed = true;
                else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Right)
                    moveDirection = Direction::Forward;
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Left)
                    moveDirection = Direction::Backward;

                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::X)
                    isXPressed = false;
                else if (event.key.code == sf::Keyboard::Y)
                    isYPressed = false;
                else if (event.key.code == sf::Keyboard::Z)
                    isZPressed = false;
                else if (event.key.code == sf::Keyboard::Up ||
                         event.key.code == sf::Keyboard::Right ||
                         event.key.code == sf::Keyboard::Down ||
                         event.key.code == sf::Keyboard::Left)
                    isMoving = false;
                break;
            }

            if (isMoving)
            {
                scene.moveConvert(moveAxis, moveDirection, dt);
                // scene.modelConvert();
            }

            if (!isDrawed)
                draw();

            dt = clock.restart().asMilliseconds();
        }
    }
}

void MainWindow::draw()
{
    Timer::start();

    window.clear();

    auto xSize = scene.cGetCamera().cGetResolution().x;
    auto ySize = scene.cGetCamera().cGetResolution().y;

    std::fill(pixels, pixels + xSize * ySize * 4, 0x00u);

    for (auto el : scene.cGetObjInfo().cGetVertices())
    {
        if (el.getX() < 0 ||
            el.getX() > xSize ||
            el.getY() < 0 ||
            el.getY() > ySize)
            continue;

        int x = std::trunc(el.cGetX());
        int y = std::trunc(el.cGetY());

        pixels[4 * (y * xSize + x)] = 255;     // R
        pixels[4 * (y * xSize + x) + 1] = 255; // G
        pixels[4 * (y * xSize + x) + 2] = 0;   // B
        pixels[4 * (y * xSize + x) + 3] = 255; // A
    }

    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();

    isDrawed = true;

    Timer::stop();
}
