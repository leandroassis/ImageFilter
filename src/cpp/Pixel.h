#pragma once

class Pixel {
    private:
        int red, green, blue;

    public:
        Pixel(int, int, int);
        int getRed();
        int getGreen();
        int getBlue();
}