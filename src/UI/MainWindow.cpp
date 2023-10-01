#include <iostream>
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
    window.setFramerateLimit(60);
    buffer.create(scene.cGetCamera().cGetResolution().x, scene.cGetCamera().cGetResolution().y, sf::Color::Black);
}

void MainWindow::startLoop()
{
    scene.modelConvert();
    draw();

    Timer::stop();

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
                if (event.key.code == sf::Keyboard::X)
                    isXPressed = true;
                else if (event.key.code == sf::Keyboard::Y)
                    isYPressed = true;
                else if (event.key.code == sf::Keyboard::Z)
                    isZPressed = true;
                else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Right)
                {
                    moveDirection = Direction::Forward;

                    if (isXPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::X;
                        isDrawed = false;
                    }
                    else if (isYPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::Y;
                        isDrawed = false;
                    }
                    else if (isZPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::Z;
                        isDrawed = false;
                    }
                }
                else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Left)
                {
                    moveDirection = Direction::Backward;

                    if (isXPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::X;
                        isDrawed = false;
                    }
                    else if (isYPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::Y;
                        isDrawed = false;
                    }
                    else if (isZPressed)
                    {
                        isMoving = true;
                        moveAxis = AxisName::Z;
                        isDrawed = false;
                    }
                }

                break;
            case sf::Event::KeyReleased:
                if (event.key.code == sf::Keyboard::X ||
                    event.key.code == sf::Keyboard::Z ||
                    event.key.code == sf::Keyboard::Z ||
                    event.key.code == sf::Keyboard::Up ||
                    event.key.code == sf::Keyboard::Right ||
                    event.key.code == sf::Keyboard::Down ||
                    event.key.code == sf::Keyboard::Left)
                    isMoving = false;

                break;
            }
        }

        if (isMoving)
            scene.moveConvert(moveAxis, moveDirection);

        if (!isDrawed)
            draw();
    }
}

void MainWindow::draw()
{
    std::cout << "draw: "
              << scene.cGetObjInfo().cGetVertices().begin()->getX() << " "
              << scene.cGetObjInfo().cGetVertices().begin()->getY() << " "
              << scene.cGetObjInfo().cGetVertices().begin()->getZ() << " "
              << scene.cGetObjInfo().cGetVertices().begin()->getW().value_or(1)
              << std::endl;

    window.clear();

    auto px = const_cast<sf::Uint8 *>(buffer.getPixelsPtr());
    std::fill(px, px + buffer.getSize().x * buffer.getSize().y * 4, 0x00u);

    for (auto el : scene.cGetObjInfo().cGetVertices())
    {
        // std::cout << el.getX() << ' ' << el.getY() << ' ' << el.getZ() << std::endl;

        if (el.getX() < 0 ||
            el.getX() > scene.cGetCamera().cGetResolution().x ||
            el.getY() < 0 ||
            el.getY() > scene.cGetCamera().cGetResolution().y)
            continue;

        buffer.setPixel(el.getX(), el.getY(), sf::Color::Red);
    }

    bufferTexture.loadFromImage(buffer);
    bufferSprite.setTexture(bufferTexture);
    window.draw(bufferSprite);
    window.display();

    isDrawed = true;
}
