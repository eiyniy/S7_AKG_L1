#include <Engine.hpp>

Engine::Engine(Scene &p_scene, MainWindow &p_mainWindow)
    : scene(p_scene),
      mainWindow(p_mainWindow),
      isCameraMoving(false),
      isObjectMoving(false),
      isCameraRotating(false),
      isCameraRotatingAround(false),
      isCentering(false)
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
    mainWindow.getWindow().pollEvent(event);

    switch (event.type)
    {
    case sf::Event::Closed:
        mainWindow.getWindow().close();
        break;
    case sf::Event::Resized:
    {
        mainWindow.resize(event.size.width, event.size.height);
        scene.resize(event.size.width, event.size.height);
        break;
    }
    case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Up ||
            event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::Left)
        {
            if (event.key.control && !event.key.alt)
                isObjectMoving = true;
            else if (!event.key.control && event.key.alt)
                isCameraRotating = true;
            else if (event.key.control && event.key.alt)
                isCameraRotatingAround = true;
            else if (!event.key.control && !event.key.alt)
                isCameraMoving = true;
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
        else if (event.key.code == sf::Keyboard::C && event.key.control)
            isCentering = true;
        else if (event.key.code == sf::Keyboard::F11)
        {
            mainWindow.switchVideoMode();
            scene.resize(mainWindow.cGetResolution().cGetX(), mainWindow.cGetResolution().cGetY());
        }
        else if (event.key.code == sf::Keyboard::Escape)
        {
            mainWindow.switchVideoMode(true);
            scene.resize(mainWindow.cGetResolution().cGetX(), mainWindow.cGetResolution().cGetY());
        }

        break;

    case sf::Event::KeyReleased:
        if (event.key.code == sf::Keyboard::Up ||
            event.key.code == sf::Keyboard::Right ||
            event.key.code == sf::Keyboard::Down ||
            event.key.code == sf::Keyboard::Left)
        {
            isCameraMoving = false;
            isCameraRotating = false;
            isCameraRotatingAround = false;
            isObjectMoving = false;
        }
        else if (event.key.code == sf::Keyboard::C)
            isCentering = false;

        break;
    }
}

void Engine::update()
{
    if (isCameraMoving)
    {
        auto transition = scene.getTransition(moveAxis, moveDirection, dt);
        scene.moveCamera(transition);
    }
    else if (isCameraRotatingAround)
    {
        scene.rotateCameraAround(moveAxis, moveDirection, dt);
    }
    else if (isObjectMoving)
    {
        auto transition = scene.getTransition(moveAxis, moveDirection, dt);
        scene.moveObject(scene.getSelectedObjectName(), transition);
    }
    else if (isCentering)
    {
        scene.centralizeCamera();
    }

    if (isCameraMoving || isCameraRotatingAround || isObjectMoving || isCentering)
        scene.convertAllModels();
}

void Engine::draw()
{
    mainWindow.clear();

    mainWindow.drawModel(
        *scene.getObject(scene.floorObjectName),
        scene.getObjectConvertedVertices(scene.floorObjectName));

    for (auto key : scene.getAllObjectNames())
    {
        if (key == scene.floorObjectName)
            continue;

        mainWindow.drawModel(*scene.getObject(key), scene.getObjectConvertedVertices(key));
    }

    mainWindow.drawPixels();

    dt = clock.restart().asMilliseconds();
}
