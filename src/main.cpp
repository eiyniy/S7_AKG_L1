#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parser = new ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    auto tsStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto objInfo = parser->parseEntries();

    auto tsEndParse = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    auto tsEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    cout << "Vertices count - " << objInfo->getVertices().size() << endl;
    cout << "Texture vertices count - " << objInfo->getTVertices().size() << endl;
    cout << "Normal vertices count - " << objInfo->getNVertices().size() << endl;
    cout << "Polygons count - " << objInfo->getPolygons().size() << endl;
    cout << endl;
    cout << "Parse time - " << tsEndParse - tsStart << "ms" << endl;
    cout << "Total computing time - " << tsEnd - tsStart << "ms" << endl;

    return 0;
}