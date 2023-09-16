#include <iostream>
#include <ObjParser.hpp>
#include <Vertex.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parser = ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    auto vertex = parser.parseVertex("v  -93.8845 599.1862 -119.7966");

    vertex.print();

    return 0;
}