#include <iostream>
#include <Magick++.h>
#include "ImageProcessing.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* const argv[]){
    srand(time(NULL));

    ImageProcessing img, img2;

    img = ImageProcessing("logo.ppm");

    img.grayscale();
    img2 = img;
    img2.resize(100, 100);

    img2.save("logo_grayscale.png");

    img2.denoise();


    return 0;
}