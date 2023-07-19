#include <iostream>
#include <Magick++.h>
#include "ImageProcessing.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* const argv[]){
    ImageProcessing img;

    img = ImageProcessing("logo.ppm");
    img.getPixels();

    return 0;
}