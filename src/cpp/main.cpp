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

    img = ImageProcessing("logo.ppm");

    img.grayscale();
    img2 = img;

    img2.save("logo_grayscale.png");
    
    img.mirror(1, "horizontal.png");

    Py_Finalize();

    return 0;
}