#include <iostream>
#include <chrono>
#include <ObjParser.hpp>
#include <Vertex.hpp>
#include <Types.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parserPt = new ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    auto tsStart = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    auto objInfoPt = parserPt->parseEntries(ParseType::Iterator);
    auto tsEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    cout << "Iterator parse time - " << tsEnd - tsStart << "ms" << endl;
    cout << endl;

    cout << "Vertices count - " << objInfoPt->getVertices().size() << endl;
    cout << "Texture vertices count - " << objInfoPt->getTVertices().size() << endl;
    cout << "Normal vertices count - " << objInfoPt->getNVertices().size() << endl;
    cout << "Polygons count - " << objInfoPt->getPolygons().size() << endl;

    delete objInfoPt;
    delete parserPt;

    return 0;
}