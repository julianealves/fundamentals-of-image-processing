#include "mainwindow.h"
#include "photoprocessor.cpp"
using namespace std;

int main(int argc, char *argv[])
{
    PhotoProcessor photo_processor;

    photo_processor = PhotoProcessor();
    photo_processor.open_image(argc, argv);

    return 0;
}
