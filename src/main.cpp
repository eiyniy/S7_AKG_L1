#include <iostream>
#include <future>
#include <ObjParser.hpp>
#include <Timer.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <Engine.hpp>

int main(int argc, char **argv) {
    std::cout << "Hello world!" << std::endl;

//    auto parser = ObjParser(argv[1]);
//    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/untitled.obj");
//    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/rubik.obj");
//    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/angel2.obj");
    auto parser = ObjParser("C:/Users/Natallia/Documents/Labs/AKG/L1/resources/models/Z3_OBJ.obj");
    auto objInfoPtFuture = std::async(
            std::launch::async,
            [&parser] {
                auto str = parser.readFile();
                return parser.parseEntries(str);
            });

    auto videoMode = sf::VideoMode::getDesktopMode();
    auto cameraResolution = Point(videoMode.width, videoMode.height);
    auto camera = Camera({0, 1, 0}, {1, 1, 1}, {0, 0, 0}, cameraResolution, 100);
    auto scene = Scene(camera, 0.5, 0.5);
    auto mainWindow = MainWindow(cameraResolution);
    auto engine = Engine(scene, mainWindow);

    auto objInfoPt = objInfoPtFuture.get();

    scene.addObject("MainObject", objInfoPt);

    /*
        std::vector<Vertex> testVertices = {
            {100, 100, 0, 1, false},
            {200, 200, 0, 1, false},
            {100, 300, 0, 1, false}};
        Polygon testPolygon{{{1}, {2}, {3}}};
        std::vector<Point> testClipper = {
            {0, 150},
            {400, 150},
            {400, 250},
            {0, 250}};
    */

    // SHClipper::clip(testPolygon, testVertices, testClipper);

    engine.start();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    return 0;
}