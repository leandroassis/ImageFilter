#include "Pixel.h"

Pixel::Pixel(int red, int green, int blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
}

int Pixel::getRed() {
    return this->red;
}

int Pixel::getGreen() {
    return this->green;
}

int Pixel::getBlue() {
    return this->blue;
}