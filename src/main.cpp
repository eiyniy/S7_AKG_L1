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
    auto vertices = parser->parseVertices();
    auto tsEndParse = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    cout << "Vertices count - " << vertices->size() << endl;
    cout << "Parse time - " << tsEndParse - tsStart << "ms" << endl;

    auto tsEnd = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();

    cout << "Total computing time - " << tsEnd - tsStart << "ms" << endl;

    return 0;
}