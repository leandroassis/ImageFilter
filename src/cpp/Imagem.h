#pragma once

#include <python3.10/Python.h>
#include <Magick++.h>
#include <string>
#include <vector>
#include "Pixel.h"

using namespace std;
using namespace Magick;

class Imagem : public Image {
    private:
        string path;
        int altura, largura;

        vector<Pixel> pixels;

    public:
        Imagem(string);
        ~Imagem();

        // getters e setters
        string getPath();
        int getAltura();
        int getLargura();
        void getPixels();

        // funções override de Magick::Image

        // funções membro

        // operadores
}