#include <iostream>
#include <MainWindow.hpp>

MainWindow::MainWindow(Scene &p_scene)
    : window({sf::VideoMode(640, 480), "SFML Graphics"}),
      scene(p_scene)
{
    window.setFramerateLimit(165);
    buffer.create(640, 480, sf::Color::Black);
}

void MainWindow::startLoop()
{
    window.clear();

    for (auto el : scene.getObjInfo().getVertices())
    {
        // std::cout << el.getX() << ' ' << el.getY() << ' ' << el.getZ() << std::endl;

        if (el.getX() < 0 || el.getX() > 640 || el.getY() < 0 || el.getY() > 480)
            continue;

        buffer.setPixel(el.getX(), el.getY(), sf::Color::Red);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bufferTexture.loadFromImage(buffer);
        bufferSprite.setTexture(bufferTexture);
        window.draw(bufferSprite);

        window.display();
    }
}
