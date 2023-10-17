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
    while (mainWindow.getWindow().isOpen())
    {
        handleEvents();
        update();
        draw();
    }
}

Matrix<4, 1> Engine::getTransition(
    const AxisName axis,
    const Direction direction,
    const double step)
{
    Matrix<4, 1> transition;

    switch (axis)
    {
    case AxisName::X:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(step, 0, 0, 0);
        else
            transition = Matrix<4, 1>(-step, 0, 0, 0);

        break;
    case AxisName::Y:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, step, 0, 0);
        else
            transition = Matrix<4, 1>(0, -step, 0, 0);

        break;
    case AxisName::Z:
        if (direction == Direction::Forward)
            transition = Matrix<4, 1>(0, 0, step, 0);
        else
            transition = Matrix<4, 1>(0, 0, -step, 0);

        break;
    }

    return transition;
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
    {
        const double ratio = dt != 0 ? (dt / scene.defaultFrameTime) : 1.f;

        if (event.key.control && !event.key.alt)
        {
            const double step = scene.moveSpeed * ratio;

            commandsQueue.push(std::move(std::make_unique<MoveObjectCommand>(
                MoveObjectCommand(
                    *scene.getObject(scene.cGetSelectedObjectName()),
                    moveAxis,
                    moveDirection,
                    step))));
        }
        else if (event.key.control && event.key.alt)
        {
            const double step = scene.rotationSpeed * ratio;

            commandsQueue.push(std::move(std::make_unique<RotateCameraAroundCommand>(
                RotateCameraAroundCommand(
                    scene.getCamera(),
                    moveAxis,
                    moveDirection,
                    step))));
        }
        else if (!event.key.control && !event.key.alt)
        {
            const double step = scene.rotationSpeed * ratio;

            commandsQueue.push(std::move(std::make_unique<MoveCameraCommand>(
                MoveCameraCommand(
                    scene.getCamera(),
                    moveAxis,
                    moveDirection,
                    step))));
        }
        break;
    }
    case sf::Keyboard::C:
        if (!event.key.control)
            break;

        commandsQueue.push(std::move(std::make_unique<CentralizeCameraCommand>(
            CentralizeCameraCommand(
                scene.getCamera(),
                *scene.getObject(scene.cGetSelectedObjectName())))));

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
    while (auto command = commandsQueue.tryPop())
        (*command)->execute();
}

void Engine::draw()
{
    mainWindow.clear();

    mainWindow.drawModel(
        *scene.cGetObject(scene.floorObjectName),
        scene.cGetObject(scene.floorObjectName)->cGetDrawable(scene.cGetCamera()));

    for (auto key : scene.cGetAllObjectNames())
    {
        if (key == scene.floorObjectName)
            continue;

        mainWindow.drawModel(
            *scene.cGetObject(key),
            scene.cGetObject(key)->cGetDrawable(scene.cGetCamera()));
    }

    mainWindow.drawPixels();

    dt = clock.restart().asMilliseconds();
}
