#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>

MainWindow::MainWindow(Scene &p_scene)
    : window({sf::VideoMode(p_scene.cGetCamera().cGetResolution().x,
                            p_scene.cGetCamera().cGetResolution().y),
              "SFML Graphics"}),
      scene(p_scene),
      isDrawed(false)
{
    window.setFramerateLimit(165);
    buffer.create(scene.cGetCamera().cGetResolution().x, scene.cGetCamera().cGetResolution().y, sf::Color::Black);
}

void MainWindow::startLoop()
{
    window.clear();

    scene.modelConvert();
    draw();

    Timer::stop();

    while (window.isOpen())
    {
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
            }
        }

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

    for (auto el : scene.cGetObjInfo().cGetVertices())
    {
        // std::cout << el.getX() << ' ' << el.getY() << ' ' << el.getZ() << std::endl;

        if (el.getZ() < 0 ||
            el.getZ() > scene.cGetCamera().cGetResolution().y ||
            el.getY() < 0 ||
            el.getY() > scene.cGetCamera().cGetResolution().x)
            continue;

        buffer.setPixel(el.getY(), el.getZ(), sf::Color::Red);
    }

    bufferTexture.loadFromImage(buffer);
    bufferSprite.setTexture(bufferTexture);
    window.draw(bufferSprite);
    window.display();

    isDrawed = true;
}
