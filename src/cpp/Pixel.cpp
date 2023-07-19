#include "Pixel.h"

// construtores e destrutores
// @brief: construtor padrão, gera um objeto nulo.
Pixel::Pixel(){
    this->red = 0;
    this->green = 0;
    this->blue = 0;
    this->opacity = 0;
}

// @brief: construtor que recebe os valores de cada canal de cor e a opacidade.
Pixel::Pixel(int red, int green, int blue, float opacity){
    this->red = red;
    this->green = green;
    this->blue = blue;
    this->opacity = opacity;
}

// @brief: construtor que recebe um pixel e copia seus valores.
Pixel::Pixel(Pixel &pixel){
    this->red = pixel.red;
    this->green = pixel.green;
    this->blue = pixel.blue;
    this->opacity = pixel.opacity;
}

// @brief: destrutor padrão.
Pixel::~Pixel(){
    // nada alocado dinamicamente
}

// getters e setters
// @brief: retorna o valor do canal de cor vermelho.
int Pixel::getRed(){
    return this->red;
}

// @brief: retorna o valor do canal de cor verde.
int Pixel::getGreen(){
    return this->green;
}

// @brief: retorna o valor do canal de cor azul.
int Pixel::getBlue(){
    return this->blue;
}

// @brief: retorna o valor da opacidade.
float Pixel::getOpacity(){
    return this->opacity;
}

// @brief: altera o valor do canal de cor vermelho.
void Pixel::setRed(int red){
    if(red < 0)
        this->red = 0;
    else if(red > 255)
        this->red = this->red % 255;
    else
        this->red = red;
}

// @brief: altera o valor do canal de cor verde.
void Pixel::setGreen(int green){
    if(green < 0)
        this->green = 0;
    else if(green > 255)
        this->green = this->green % 255;
    else
        this->green = green;
}

// @brief: altera o valor do canal de cor azul.
void Pixel::setBlue(int blue){
    if(blue < 0)
        this->blue = 0;
    else if(blue > 255)
        this->blue = this->blue % 255;
    else
        this->blue = blue;
}

// @brief: altera o valor da opacidade.
void Pixel::setOpacity(float opacity){
    if(opacity < 0)
        this->opacity = 0.0;
    else if(opacity > 1)
        this->opacity = 1.0;
    else
        this->opacity = opacity;
}

// operadores
// @brief: soma dois pixels, retornando um novo pixel com a soma dos valores de cada canal de cor.
Pixel& Pixel::operator+(Pixel &pixel){
    Pixel *newPixel = new Pixel();
    newPixel->setRed(this->getRed() + pixel.getRed());
    newPixel->setGreen(this->getGreen() + pixel.getGreen());
    newPixel->setBlue(this->getBlue() + pixel.getBlue());
    newPixel->setOpacity(this->getOpacity() + pixel.getOpacity());
    return *newPixel;
}

// @brief: soma dois pixels, alterando o valor do pixel da esquerda.
Pixel& Pixel::operator+=(Pixel &pixel){
    this->setRed(this->getRed() + pixel.getRed());
    this->setGreen(this->getGreen() + pixel.getGreen());
    this->setBlue(this->getBlue() + pixel.getBlue());
    this->setOpacity(this->getOpacity() + pixel.getOpacity());
    return *this;
}

Pixel& Pixel::operator*(Pixel &pixel){
    Pixel *newPixel = new Pixel();
    newPixel->setRed(this->getRed() * pixel.getRed());
    newPixel->setGreen(this->getGreen() * pixel.getGreen());
    newPixel->setBlue(this->getBlue() * pixel.getBlue());
    newPixel->setOpacity(this->getOpacity() * pixel.getOpacity());
    return *newPixel;
}

int Pixel::operator[](int indice){
    if(indice == 0)
        return this->red;
    else if(indice == 1)
        return this->green;
    else if(indice == 2)
        return this->blue;
    else if(indice == 3)
        return this->opacity;
    else
        return -1;
}