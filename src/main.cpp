#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Timer.hpp>
#include <Matrix.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>
#include <MatrixStaticStorage.hpp>

int main(int argc, char **argv)
{
    Timer::start();

    std::cout << "Hello world!" << std::endl;

    auto parser = ObjParser(argv[1]);

    auto tsStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string *fileContentPt = parser.readFile();

    auto objInfoPt = parser.parseEntries(*fileContentPt);

    std::cout << "Vertices count - " << objInfoPt->cGetVertices().size() << std::endl;
    std::cout << "Texture vertices count - " << objInfoPt->cGetTVertices().size() << std::endl;
    std::cout << "Normal vertices count - " << objInfoPt->cGetNVertices().size() << std::endl;
    std::cout << "Polygons count - " << objInfoPt->cGetPolygons().size() << std::endl;
    std::cout << std::endl;

    auto tsParseEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Parse time - " << tsParseEnd - tsStart << "ms" << std::endl;

    auto camera = Camera({0, 0, 0}, {objInfoPt->cGetVertices().at(0)}, {1280, 720}, 100);
    auto up = CoordinateVector(0, 1, 0);
    auto scene = Scene(*objInfoPt, camera, up);

    auto mainWindow = MainWindow(scene);
    mainWindow.startLoop();

    delete objInfoPt;

    std::cout << "Timer time - " << Timer::getNs() / 1000000 << "ms. Calls count - " << Timer::getCalls() << std::endl;

    return 0;
}