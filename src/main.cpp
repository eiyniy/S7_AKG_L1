#include <iostream>
#include <future>
#include <ObjParser.hpp>
#include <Timer.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <Engine.hpp>
#include <DirectLight.hpp>
#include <SpotLight.hpp>
#include <LambertModel.hpp>
#include <PhongModel.hpp>
#include <ImageParser.hpp>
#include <Texture.hpp>
#include <Globals.hpp>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    //    auto parser = ObjParser(argv[1]);

    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/mimic/mimic.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/boll/export3dcoat.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Tree Man/model.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Robot Steampunk/robot_steampunk.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Intergalactic Spaceship/Intergalactic_Spaceship-(Wavefront).obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Cyber Mancubus/mancubus.obj");
    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Doom_Slayer/doomslayer.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/knight/shovel_low.obj");

    /*
    auto parser = ObjParser(
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/chess/chess2.obj",
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/chess/ChessDiffuse.png",
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/chess/Normal2.png");
    */

    /*
    auto parser = ObjParser(
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/head/Head.obj",
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/head/Head_diffuse.png",
        "C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/head/Head_normal_map.png");
    */

    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/untitled.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/rubik.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/cat2.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/woman1.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/woman2.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/angel2.obj");
    // auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Z3_OBJ.obj");

    // /*
    auto objInfoPtFuture = std::async(
        std::launch::async,
        [&parser]
        {
            return parser.parse();
        });
    // */

    // const auto str = parser.readFile();
    // auto objInfoPt = parser.parse(str);

    const auto videoMode = sf::VideoMode::getDesktopMode();
    Point cameraResolution{(int)videoMode.width, (int)videoMode.height};
    Matrix<4, 1> cameraPosition{15, 40, 15};
    Camera camera{{0, 1, 0}, cameraPosition, {0, 2, 0}, cameraResolution, 100};
    Matrix<4, 1> lightPos{10, 15, 50};
    // Matrix<4, 1> lightDirection{-0.5, -2, -1};
    // lightDirection.normalize();
    // DirectLight lightSource{lightDirection};
    SpotLight lightSource{lightPos};
    // LambertModel lightingModel{};
    PhongModel lightingModel{0.1, 1, 32};
    Scene scene{camera, &lightSource, 2, 2};
    MainWindow mainWindow{cameraResolution, &lightingModel, scene.cGetLightSource(), ShadingModel::Phong};
    Engine engine{scene, mainWindow};

    auto objInfoPt = objInfoPtFuture.get();
    scene.addObject("MainObject", objInfoPt);

    engine.start();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    return 0;
}