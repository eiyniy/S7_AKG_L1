#include <Engine.hpp>
#include <MoveCameraCommand.hpp>
#include <MoveObjectCommand.hpp>
#include <RotateCameraAroundCommand.hpp>
#include <CentralizeCameraCommand.hpp>
#include <SwitchVideoModeCommand.hpp>
#include <CloseCommand.hpp>
#include <ResizeCommand.hpp>
#include <ThreadPool.hpp>
#include <Timer.hpp>
#include <LambertModel.hpp>

Engine::Engine(Scene &_scene, MainWindow &_mainWindow)
    : scene(_scene),
      mainWindow(_mainWindow)
{
    //    mainWindow.getWindow().setFramerateLimit(defaultFps);

    draw();
}

void Engine::start()
{
    for (const auto &key : scene.cGetAllObjectNames())
        scene.getObject(key)->convertToDrawable(scene.cGetCamera());

    // auto ts = std::chrono::high_resolution_clock::now();
    // auto te = std::chrono::high_resolution_clock::now();

    while (mainWindow.getWindow().isOpen())
    {
        // ts = std::chrono::high_resolution_clock::now();
        handleEvents();
        // te = std::chrono::high_resolution_clock::now();
        // const auto inputMs = std::chrono::duration_cast<std::chrono::milliseconds>(te - ts).count();

        // ts = std::chrono::high_resolution_clock::now();
        update();
        // te = std::chrono::high_resolution_clock::now();
        // const auto updateMs = std::chrono::duration_cast<std::chrono::milliseconds>(te - ts).count();

        // ts = std::chrono::high_resolution_clock::now();
        draw();
        // te = std::chrono::high_resolution_clock::now();
        // const auto drawMs = std::chrono::duration_cast<std::chrono::milliseconds>(te - ts).count();

        //        std::cout << "Input time: " << inputMs << " ms" << std::endl;

        // std::cout << "Update time: " << updateMs << " ms" << std::endl;
        // std::cout << "Draw time: " << drawMs << " ms" << std::endl;
        // std::cout << "Timer frame time: " << Timer::getMcs() / 1000 << " ms" << std::endl; // 26 ms

        // std::cout << "Frame time: " << inputMs + updateMs + drawMs << " ms" << std::endl;

        // std::cout << std::endl;

        // Timer::reset();
    }
}

void Engine::handleEvents()
{
    static sf::Event event;
    const auto isPooled = mainWindow.getWindow().pollEvent(event);

    if (!isPooled)
        return;

    switch (event.type)
    {
    case sf::Event::Closed:
        commandsQueue.emplace(std::make_unique<CloseCommand>(mainWindow));
        break;
    case sf::Event::Resized:
        commandsQueue.emplace(std::make_unique<ResizeCommand>(
            scene.getCamera(),
            mainWindow,
            event.size.width,
            event.size.height));
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

            commandsQueue.emplace(std::make_unique<MoveObjectCommand>(
                *scene.getObject(scene.cGetSelectedObjectName()),
                moveAxis,
                moveDirection,
                step));
        }
        else if (event.key.control && event.key.alt)
        {
            const double step = scene.rotationSpeed * ratio;

            commandsQueue.emplace(std::make_unique<RotateCameraAroundCommand>(
                scene.getCamera(),
                moveAxis,
                moveDirection,
                step));
        }
        else if (!event.key.control && !event.key.alt)
        {
            const double step = scene.moveSpeed * ratio;

            commandsQueue.emplace(std::make_unique<MoveCameraCommand>(
                scene.getCamera(),
                moveAxis,
                moveDirection,
                step));
        }
        break;
    }
    case sf::Keyboard::C:
        if (!event.key.control)
            break;

        commandsQueue.emplace(std::make_unique<CentralizeCameraCommand>(
            scene.getCamera(),
            *scene.getObject(scene.cGetSelectedObjectName())));

        break;
    case sf::Keyboard::F11:
    case sf::Keyboard::Escape:
        commandsQueue.emplace(std::make_unique<SwitchVideoModeCommand>(
            scene.getCamera(),
            mainWindow,
            event.key.code == sf::Keyboard::Escape));
        break;
    }
}

void Engine::update()
{
    if (commandsQueue.empty())
        return;

    //    while (!commandsQueue.empty()) {
    const auto command = std::move(commandsQueue.front());
    commandsQueue.pop();
    command->execute();
    //    }

    for (const auto &key : scene.cGetAllObjectNames())
        scene.getObject(key)->convertToDrawable(scene.cGetCamera());
}

void Engine::draw()
{
    mainWindow.clear();

    // mainWindow.drawModel(
    //     *scene.getObject(scene.floorObjectName),
    //     scene.getObject(scene.floorObjectName)->cGetDrawable(scene.cGetCamera()));

    const auto &cameraPosition = scene.cGetCamera().cGetPosition();

    for (const auto &key : scene.cGetAllObjectNames())
    {
        auto object = scene.getObject(key);

        // if (key == scene.floorObjectName)
        // continue;

        mainWindow.drawModel(*object, cameraPosition);
    }

    mainWindow.drawPixels();

    dt = clock.restart().asMilliseconds();
}
