#include <iostream>
#include <Magick++.h>
#include "ImageProcessing.h"

using namespace std;
using namespace Magick;

int main(int argc, const char* const argv[]){
    ImageProcessing img;

    img = ImageProcessing("logo.ppm");
    cout << *(img[11][50]) << endl;

    img.grayscale();
    img.save("logo_grayscale.png");

    cout << *(img[11][50]) << endl;

    return 0;
}