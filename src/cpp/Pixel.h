#pragma once

#include <vector>

using namespace std;

class Pixel {
    private:
        int red, green, blue;
        int opacity;
    
    public:
        // construtores e destrutores
        Pixel();
        Pixel(int, int, int);
        Pixel(int, int, int, int);
        Pixel(Pixel &);
        ~Pixel();

        // getters e setters
        int getRed();
        int getGreen();
        int getBlue();
        int getOpacity();
        void setRed(int);
        void setGreen(int);
        void setBlue(int);
        void setOpacity(int);

        // operadores
        // + para somar dois pixels
        // * para multiplicar um pixel por um inteiro
        Pixel& operator+(const Pixel&);
        Pixel& operator*(int);
        vector<int>& operator[](int);
};