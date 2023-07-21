#pragma once

#include <iostream>
#include <vector>

using namespace std;

class Pixel {
    friend ostream &operator<<(ostream &, const Pixel&);
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
        Pixel& operator+(Pixel&);
        Pixel& operator+=(Pixel&);
        Pixel& operator*(Pixel&);
        int operator[](int);
        
};