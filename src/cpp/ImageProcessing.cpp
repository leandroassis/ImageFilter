#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "ImageProcessing.h"

// construtores e destrutores
// @brief: construtor padrão, gera um objeto nulo.
ImageProcessing::ImageProcessing() {
    this->path = "";
    this->altura = 0;
    this->largura = 0;
}

// @brief: construtor que recebe o caminho da imagem. Preenche os atributos da classe com as informações da imagem.
ImageProcessing::ImageProcessing(string path) {

    srand(time(NULL));
    char temp[20];

    // salva o caminho da imagem
    this->path = path;
    
    // tenta abrir a imagem
    try{
        this->MagickImage.read(this->path);
    } 
    catch (Magick::ErrorFileOpen &error) {
        cout << "Erro durante a criação da classe ImageProcessing: " << error.what() << endl;
        cout << "Verifique se o caminho da imagem está correto." << endl;
        return;
    }
    catch (Magick::ErrorCorruptImage &error){
        cout << "Erro durante a criação da classe ImageProcessing: " << error.what() << endl;
        cout << "Verifique se a imagem está corrompida." << endl;
        return;
    }
    catch (...){
        cout << "Erro desconhecido durante a criação da classe ImageProcessing." << endl;
        return;
    }

    // pega os atributos da imagem de entrada
    this->altura = this->MagickImage.rows();
    this->largura = this->MagickImage.columns();

    
    // converte a image para um arquivo ppm para pegar os pixels
    try{
        for(int i = 0; i < 20; i++){
            temp[i] = rand() % 26 + 97;
        }
        this->MagickImage.write("./tmp/" + string(temp) + ".ppm");   
    }
    catch(...){
        cout << "Erro ao converter a imagem para um arquivo ppm." << endl;
        return;
    }

    // le todos os pixels da imagem e constroi o vetor de pixels
    ifstream ppm_image("./tmp/" + string(temp) + ".ppm");

    if(!this->ppmToVector(&ppm_image)){
        cout << "Erro ao gerar vetor de pixels da imagem." << endl;
        return;
    }

    ppm_image.close();

    try{
        remove(("./tmp/" + this->path + ".ppm").c_str());
    }
    catch(...){
        cout << "Erro ao remover o arquivo temporário." << endl;
        return;
    }
}

// @brief: construtor que recebe um pixel padrão e as dimensões da imagem. A imagem saída será totalmente preenchida com o pixel padrão.
ImageProcessing::ImageProcessing(Pixel std_value, int largura, int altura){
    this->path = "";
    this->largura = largura;
    this->altura = altura;

    for(int i = 0; i < this->altura; i++){
        for(int j = 0; j < this->largura; j++){
            this->pixels.push_back(new Pixel(std_value));
        }
    }

    this->MagickImage = Image(Geometry(this->largura, this->altura), ColorRGB(std_value.getRed(), std_value.getGreen(), std_value.getBlue()));
}

// @brief: construtor que recebe as dimensões da imagem e um modo.
//         O modo pode ser:
//         "random" - gera uma imagem com pixels aleatórios
//         "black" - gera uma imagem preta
//         "white" - gera uma imagem branca
//         "fractal" - gera uma imagem com um padrão de fractal preto e branco
ImageProcessing::ImageProcessing(int largura, int altura, string modo){

    char temp[20];
    int r, g, b;

    if(modo != "random" && modo != "black" && modo != "white" && modo != "fractal"){
        cout << "ImageProcessing: Modo " + modo + " inválido." << endl;
        return;
    }

    srand(time(NULL));

    this->path = "";
    this->largura = largura;
    this->altura = altura;

    // gera um nome para o novo arquivo ppm
    for(int i = 0; i < 20; i++){
        temp[i] = rand() % 26 + 97;
    }

    // cria o arquivo ppm
    path = "new_" + string(temp) + ".ppm";
    ofstream ppm_image("./tmp/" + path);

    ppm_image << "P6" << endl;
    ppm_image << largura << " " << altura << endl;
    ppm_image << "255" << endl;

    if(modo == "random"){
        for(int i = 0; i < this->altura; i++){
            for(int j = 0; j < this->largura; j++){
                r = rand() % 255;
                g = rand() % 255;
                b = rand() % 255;
                this->pixels.push_back(new Pixel(r, g, b));
                ppm_image << r << " " << g << " " << b << endl;
            }
        }
    }
    else if(modo == "fractal"){
        for(int i = 0; i < this->altura; i++){
            for(int j = 0; j < this->largura; j++){
                r = (i * j) % 255;
                g = (i * j) % 255;
                b = (i * j) % 255;

                this->pixels.push_back(new Pixel(r, g, b));
                ppm_image << r << " " << g << " " << b << endl;

            }
        }
    }
    else if(modo == "black"){
        for(int i = 0; i < this->altura; i++){
            for(int j = 0; j < this->largura; j++){
                this->pixels.push_back(new Pixel(0, 0, 0));
                ppm_image << "0 0 0" << endl;
            }
        }
    }
    else if(modo == "white"){
        for(int i = 0; i < this->altura; i++){
            for(int j = 0; j < this->largura; j++){
                this->pixels.push_back(new Pixel(255, 255, 255));
                ppm_image << "255 255 255" << endl;
            }
        }
    }
    ppm_image.close();

    this->MagickImage = Image("./tmp/" + path);
}

// @brief: destrutor
ImageProcessing::~ImageProcessing(){    
    for(auto pixel : pixels){
        delete pixel;
    }
}

// getters
string ImageProcessing::getPath(){
    return this->path;
}

int ImageProcessing::getAltura(){
    return this->altura;
}

int ImageProcessing::getLargura(){
    return this->largura;
}

void ImageProcessing::getPixels(){
    for(auto pixel : this->pixels){
        cout << pixel->getRed() << " " << pixel->getGreen() << " " << pixel->getBlue() << endl;
    }
}

// setters

// funções auxiliares

// @brief: lê um arquivo ppm e transforma em um vetor de pixels da classe
int ImageProcessing::ppmToVector(ifstream *ppm_image){
    
    if(!ppm_image->is_open() || ppm_image->eof() || ppm_image->fail() || ppm_image->bad()){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        return 0;
    }

    string _file_identifier;
    *ppm_image >> _file_identifier;
    if(_file_identifier != "P6"){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        return 0;
    }

    int _altura, _largura;
    *ppm_image >> _largura >> _altura;
    *ppm_image >> this->max_color;

    if(_altura != this->altura || _largura != this->largura){
        cout << "Aviso: O arquivo PPM não possui as dimensões de altura e largura da imagem de entrada. Isso pode ocasionar erros." << endl;
    }

    int r, g, b;
    while(!ppm_image->eof()){
        *ppm_image >> r >> g >> b;

        Pixel *pixel = new Pixel();
        pixel->setRed(r);
        pixel->setGreen(g);
        pixel->setBlue(b);
        this->pixels.push_back(pixel);
    }

    return 1;
}

// @brief: transforma o vetor de pixels da classe em um numpy array
int ImageProcessing::vectorToArray(){

    return 1;
}

// @brief: salva a imagem com o nome e o formato especificados
int save(string output_path){

    return 1;
}