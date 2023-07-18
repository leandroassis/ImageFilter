#pragma once

#include <python3.10/Python.h>
#include <Magick++.h>
#include <string>
#include <vector>

using namespace std;
using namespace Magick;

class Imagem : public Image {
    private:
        string path;
        int altura, largura;

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