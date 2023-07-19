#pragma once

#include <vector>

using namespace std;

class Pixel {
    private:
        int red, green, blue;
        float opacity;
    
    public:
        // construtores e destrutores
        Pixel();
        Pixel(int, int, int, float = 0.0);
        Pixel(Pixel &);
        ~Pixel();

        // getters e setters
        int getRed();
        int getGreen();
        int getBlue();
        float getOpacity();
        void setRed(int);
        void setGreen(int);
        void setBlue(int);
        void setOpacity(float);

        // operadores
        // + para somar dois pixels
        // * para multiplicar um pixel por um inteiro
        Pixel& operator+(Pixel&);
        Pixel& operator+(int);
        Pixel& operator*(Pixel&);
        Pixel& operator*(int);
        int operator[](int);
        
};