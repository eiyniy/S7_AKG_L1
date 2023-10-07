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
      isObjectMoving(false)
{
    window.setFramerateLimit(scene.defaultFps);

    auto resolution = scene.cGetCamera().cGetResolution();

    pixels = new sf::Uint8[resolution.x * resolution.y * 4];
    bufferTexture.create(resolution.x, resolution.y);

    bufferSprite.setTexture(bufferTexture);
}

void MainWindow::startLoop()
{
    scene.modelConvert(scene.cGetObjInfoVerticesCopy());
    // scene.modelConvert(scene.getFloorCopy());

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

            scene.modelConvert(scene.cGetObjInfoVerticesCopy());
            // scene.modelConvert(scene.getFloorCopy());

            break;
        }
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Up ||
                event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::Down ||
                event.key.code == sf::Keyboard::Left)
            {
                if (!event.key.control)
                    isCameraMoving = true;
                else
                    isObjectMoving = true;

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

            break;

        case sf::Event::KeyReleased:
            if (event.key.code == sf::Keyboard::Up ||
                event.key.code == sf::Keyboard::Right ||
                event.key.code == sf::Keyboard::Down ||
                event.key.code == sf::Keyboard::Left)
            {
                isCameraMoving = false;
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
            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.cGetWorldShift());
            // scene.modelConvert(scene.getFloorCopy());
        }
        else if (isObjectMoving)
        {
            auto transition = scene.getMoveConvert(moveAxis, moveDirection, dt);

            scene.getWorldShift() = scene.getWorldShift() + transition;

            scene.modelConvert(scene.cGetObjInfoVerticesCopy(), scene.cGetWorldShift());
            // scene.modelConvert(scene.getFloorCopy());
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

    // for (auto el : scene.getDrawableFloor())
    //     window.draw(el.data(), 2, sf::Lines);

    for (auto el : scene.cGetObjInfoVertices())
    {
        int x = std::trunc(el.cGetX());
        int y = std::trunc(el.cGetY());

        pixels[4 * (y * resolution.x + x)] = 255;     // R
        pixels[4 * (y * resolution.x + x) + 1] = 255; // G
        pixels[4 * (y * resolution.x + x) + 2] = 0;   // B
        pixels[4 * (y * resolution.x + x) + 3] = 255; // A
    }

    bufferTexture.update(pixels);
    window.draw(bufferSprite);
    window.display();
}
