#include <iostream>
#include <Magick++.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "ImageProcessing.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* const argv[]){
    srand(time(NULL));
    setenv("PYTHONPATH",".",1);
    Py_Initialize();

    ImageProcessing img, img2;

    img = ImageProcessing("sample/logo.ppm");

    img.grayscale();
    img2 = img;

    img2.save("out/logo_grayscale.png");
    
    img2.mirror(1, "horizontal.png");
    img2.rotate(90, 1, "rotate.png");
    img2.to_ASCII("ascii.txt");
    img2.to_pixel("pixel.png", 4);

    Py_Finalize();

    return 0;
}