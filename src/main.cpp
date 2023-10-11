#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Timer.hpp>
#include <Matrix.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <Converter.hpp>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    auto tsStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    auto parser = ObjParser(argv[1]);

    std::string *fileContentPt = parser.readFile();

    auto objInfoPt = parser.parseEntries(*fileContentPt);

    auto tsParseEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::cout << "Vertices count - " << objInfoPt->cGetVertices().size() << std::endl;
    std::cout << "Texture vertices count - " << objInfoPt->cGetTVertices().size() << std::endl;
    std::cout << "Normal vertices count - " << objInfoPt->cGetNVertices().size() << std::endl;
    std::cout << "Polygons count - " << objInfoPt->cGetPolygons().size() << std::endl;
    std::cout << std::endl;

    std::cout << "Parse time - " << tsParseEnd - tsStart << "ms" << std::endl;

    auto cameraPosition = Matrix<4,1>(0, 0, 10, 1);
    auto cameraTarget = Matrix<4,1>(0, 0, 0, 1);
    auto cameraResolution = Dot(1280, 720);
    auto camera = Camera(cameraPosition, cameraTarget, cameraResolution, 100);
    auto up = Matrix<4,1>(0, 1, 0);
    auto scene = Scene(camera, up, 5, 0.5);
    scene.addObject("MainObject", objInfoPt);

    auto mainWindow = MainWindow(scene);
    mainWindow.startLoop();

    std::cout << "Timer time - " << Timer::getMcs() / 1000 << "ms. "
              << "Calls count - " << Timer::getCalls() << std::endl
              << std::endl;

    return 0;
}