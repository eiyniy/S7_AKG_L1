#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Timer.hpp>
#include <Matrix.hpp>
#include <MainWindow.hpp>
#include <Scene.hpp>
#include <Camera.hpp>

int main(int argc, char **argv)
{
    std::cout << "Hello world!" << std::endl;

    auto parser = ObjParser(argv[1]);

    auto tsStart = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

    std::string *fileContentPt = parser.readFile();

    auto objInfoPt = parser.parseEntries(*fileContentPt);

    std::cout << "Vertices count - " << objInfoPt->getVertices().size() << std::endl;
    std::cout << "Texture vertices count - " << objInfoPt->getTVertices().size() << std::endl;
    std::cout << "Normal vertices count - " << objInfoPt->getNVertices().size() << std::endl;
    std::cout << "Polygons count - " << objInfoPt->getPolygons().size() << std::endl;
    std::cout << std::endl;

    auto tsParseEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Parse time - " << tsParseEnd - tsStart << "ms" << std::endl;

    auto camera = Camera({0, 10, 0}, {objInfoPt->getVertices().at(1)});
    auto up = CoordinateVector(0, 1, 0);

    auto scene = Scene(*objInfoPt, camera, up);
    scene.modelConvert();

    auto mainWindow = MainWindow(scene);
    mainWindow.startLoop();

    delete objInfoPt;

    auto tsEnd = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << "Memory free time - " << tsEnd - tsParseEnd << "ms" << std::endl;
    std::cout << "Timer time - " << Timer::getNs() / 1000000 << "ms. Calls count - " << Timer::getCalls() << std::endl;
    std::cout << "Total time - " << tsEnd - tsStart << "ms" << std::endl;

    return 0;
}