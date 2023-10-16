#include <Engine.hpp>
#include <iostream>
#include <MoveCameraCommand.hpp>
#include <MoveObjectCommand.hpp>
#include <RotateCameraAroundCommand.hpp>
#include <CentralizeCameraCommand.hpp>
#include <SwitchVideoModeCommand.hpp>
#include <CloseCommand.hpp>
#include <ResizeCommand.hpp>

Engine::Engine(Scene &_scene, MainWindow &_mainWindow)
    : scene(_scene),
      mainWindow(_mainWindow)
{
    mainWindow.getWindow().setFramerateLimit(defaultFps);
    draw();
}

void Engine::start()
{
    scene.convertAllModels();

    while (mainWindow.getWindow().isOpen())
    {
        handleEvents();
        update();
        draw();
    }
}

void Engine::handleEvents()
{
    static sf::Event event;
    mainWindow.getWindow().pollEvent(event);

    switch (event.type)
    {
    case sf::Event::Closed:
        commandsQueue.push(std::move(std::make_unique<CloseCommand>(
            CloseCommand(mainWindow))));
        break;
    case sf::Event::Resized:
        commandsQueue.push(std::move(std::make_unique<ResizeCommand>(
            ResizeCommand(
                scene,
                mainWindow,
                event.size.width,
                event.size.height))));
        break;
    case sf::Event::KeyPressed:
        updateInput(event);
        sendInputCommand(event);
        break;
    }
}

void Engine::updateInput(const sf::Event &event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Up:
    case sf::Keyboard::Right:
        moveDirection = Direction::Forward;
        break;
    case sf::Keyboard::Down:
    case sf::Keyboard::Left:
        moveDirection = Direction::Backward;
        break;
    case sf::Keyboard::X:
        moveAxis = AxisName::X;
        break;
    case sf::Keyboard::Y:
        moveAxis = AxisName::Y;
        break;
    case sf::Keyboard::Z:
        moveAxis = AxisName::Z;
        break;
    }
}

void Engine::sendInputCommand(const sf::Event &event)
{
    switch (event.key.code)
    {
    case sf::Keyboard::Up:
    case sf::Keyboard::Right:
    case sf::Keyboard::Down:
    case sf::Keyboard::Left:
        if (event.key.control && !event.key.alt)
        {
            commandsQueue.push(std::move(std::make_unique<MoveObjectCommand>(
                MoveObjectCommand(
                    scene,
                    scene.cGetSelectedObjectName(),
                    moveAxis,
                    moveDirection,
                    dt))));
        }
        else if (event.key.control && event.key.alt)
        {
            commandsQueue.push(std::move(std::make_unique<RotateCameraAroundCommand>(
                RotateCameraAroundCommand(
                    scene,
                    moveAxis,
                    moveDirection,
                    dt))));
        }
        else if (!event.key.control && !event.key.alt)
        {
            commandsQueue.push(std::move(std::make_unique<MoveCameraCommand>(
                MoveCameraCommand(
                    scene,
                    moveAxis,
                    moveDirection,
                    dt))));
        }
        break;
    case sf::Keyboard::C:
        if (!event.key.control)
            break;

        commandsQueue.push(std::move(std::make_unique<CentralizeCameraCommand>(
            CentralizeCameraCommand(scene))));

        break;
    case sf::Keyboard::F11:
    case sf::Keyboard::Escape:
        commandsQueue.push(std::move(std::make_unique<SwitchVideoModeCommand>(
            SwitchVideoModeCommand(
                scene,
                mainWindow,
                event.key.code == sf::Keyboard::Escape))));
        break;
    }
}

void Engine::update()
{
    bool needUpdate = false;
    while (auto command = commandsQueue.tryPop())
    {
        (*command)->execute();
        needUpdate = true;
    }

    if (needUpdate)
        scene.convertAllModels();
}

void Engine::draw()
{
    mainWindow.clear();

    mainWindow.drawModel(
        *scene.cGetObject(scene.floorObjectName),
        scene.cGetObjectConvertedVertices(scene.floorObjectName));

    for (auto key : scene.cGetAllObjectNames())
    {
        if (key == scene.floorObjectName)
            continue;

        mainWindow.drawModel(*scene.cGetObject(key), scene.cGetObjectConvertedVertices(key));
    }

    mainWindow.drawPixels();

    dt = clock.restart().asMilliseconds();
}
