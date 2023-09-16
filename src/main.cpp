#include <iostream>
#include <ObjParser.hpp>
#include <Vertex.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parser = new ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    // auto vertexStr = "v  -93.8845 599.1862 -119.7966";
    // auto vertex = parser.parseVertex(vertexStr);
    // vertex.print();

    auto vertices = parser->parseVertices();
    cout << vertices.size();

    return 0;
}