#include <iostream>
#include <future>
#include <ObjParser.hpp>
#include <Timer.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <Engine.hpp>
#include <DirectLight.hpp>
#include <LambertModel.hpp>

int main(int argc, char **argv) {
    std::cout << "Hello world!" << std::endl;

//    auto parser = ObjParser(argv[1]);
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/untitled.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/rubik.obj");
//    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/angel2.obj");
   auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Z3_OBJ.obj");
    auto objInfoPtFuture = std::async(
            std::launch::async,
            [&parser] {
                auto str = parser.readFile();
                return parser.parseEntries(str);
            });

    const auto videoMode = sf::VideoMode::getDesktopMode();
    Point cameraResolution{(int) videoMode.width, (int) videoMode.height};
    Camera camera{{0, 1, 0}, {-10, 250, -10}, {0, 0, 0}, cameraResolution, 100};
    Matrix<4, 1> lightDirection{0, -2, -1};
    lightDirection.normalize();
    DirectLight lightSource{lightDirection};
    LambertModel lightingModel{};
    Scene scene{camera, &lightSource, 0.5, 0.5};
    MainWindow mainWindow{cameraResolution, &lightingModel, scene.cGetLightSource(), ShadingModel::Flat};
    Engine engine{scene, mainWindow};

    auto objInfoPt = objInfoPtFuture.get();
    scene.addObject("MainObject", objInfoPt);

    engine.start();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    return 0;
}