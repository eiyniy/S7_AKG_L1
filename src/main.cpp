#include <iostream>
#include <ObjParser.hpp>

using namespace std;

int main(int argc, char **argv)
{
    cout << "Hello world!" << endl;

    auto parser = ObjParser("resources/models/uploads_files_2217034_WOMAN.obj");

    return 0;
}