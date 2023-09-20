#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Timer.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parser = ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    auto tsStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    string *fileContentPt = parser.readFile();

    auto objInfoPt = parser.parseEntries(*fileContentPt);

    cout << "Vertices count - " << objInfoPt->getVertices().size() << endl;
    cout << "Texture vertices count - " << objInfoPt->getTVertices().size() << endl;
    cout << "Normal vertices count - " << objInfoPt->getNVertices().size() << endl;
    cout << "Polygons count - " << objInfoPt->getPolygons().size() << endl;
    cout << endl;

    auto tsParseEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout << "Parse time - " << tsParseEnd - tsStart << "ms" << endl;

    delete objInfoPt;

    auto tsEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    cout << "Memory free time - " << tsEnd - tsParseEnd << "ms" << endl;
    cout << "Timer time - " << Timer::getNs() / 1000000 << "ms" << endl;
    cout << "Total time - " << tsEnd - tsStart << "ms" << endl;

    return 0;
}