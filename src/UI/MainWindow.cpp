#include <iostream>
#include <MainWindow.hpp>
#include <Timer.hpp>

MainWindow::MainWindow(const Scene &p_scene)
    : window({sf::VideoMode(p_scene.getCamera().getResolution().x,
                            p_scene.getCamera().getResolution().y),
              "SFML Graphics"}),
      scene(p_scene)
{
    window.setFramerateLimit(165);
    buffer.create(scene.getCamera().getResolution().x, scene.getCamera().getResolution().y, sf::Color::Black);
}

void MainWindow::startLoop()
{
    window.clear();

    scene.modelConvert();

    for (auto el : scene.getObjInfo().getVertices())
    {
        // std::cout << el.getX() << ' ' << el.getY() << ' ' << el.getZ() << std::endl;

        if (el.getX() < 0 ||
            el.getX() > scene.getCamera().getResolution().x ||
            el.getY() < 0 ||
            el.getY() > scene.getCamera().getResolution().y)
            continue;

        buffer.setPixel(el.getX(), el.getY(), sf::Color::Red);
    }

    bufferTexture.loadFromImage(buffer);
    bufferSprite.setTexture(bufferTexture);
    window.draw(bufferSprite);
    window.display();

    Timer::stop();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }
}
