#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "ImageProcessing.h"

// construtores e destrutores
// @brief: construtor padrão, gera um objeto nulo.
ImageProcessing::ImageProcessing() : pixels(0) {
    this->path = "";
    this->altura = 0;
    this->largura = 0;
}

// @brief: construtor que recebe o caminho da imagem. Preenche os atributos da classe com as informações da imagem.
ImageProcessing::ImageProcessing(string path) {

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

    // le todos os pixels da imagem e constroi o vetor de pixels
    if(!this->ppmToVector()){
        cout << "Erro ao gerar vetor de pixels da imagem." << endl;
        return;
    }
}

// @brief: construtor que recebe um pixel padrão e as dimensões da imagem. A imagem saída será totalmente preenchida com o pixel padrão.
ImageProcessing::ImageProcessing(vector<Pixel> &std_value, int largura, int altura){
    this->path = "None";
    this->largura = largura;
    this->altura = altura;

    for(int i = 0; i < this->altura; i++){
        // aloca uma nova linha
        vector<Pixel *> *linha = new vector<Pixel *>(); 
        for(int j = 0; j < this->largura; j++){
            // cria os pixels com o respectivo valor da matriz de entrada
            Pixel *_pixel = new Pixel(std_value[i*largura + j][0], std_value[i*largura + j][1], std_value[i*largura + j][2], std_value[i*largura + j][3]);
            linha->push_back(_pixel);
        }
        // adiciona a linha no vetor de pixels
        this->pixels.push_back(*linha);
    }

    this->MagickImage = Image(Geometry(this->largura, this->altura), ColorRGB(0, 0, 0));
    for(int i = 0; i < this->altura; i++){
        for(int j = 0; j < this->largura; j++){
            this->MagickImage.pixelColor(j, i, ColorRGB(std_value[i*largura + j][0], std_value[i*largura + j][1], std_value[i*largura + j][2]));
        }
    }
}

// @brief: construtor que recebe as dimensões da imagem e um modo.
//         O modo pode ser:
//         "random" - gera uma imagem com pixels aleatórios
//         "black" - gera uma imagem preta
//         "white" - gera uma imagem branca
//         "fractal" - gera uma imagem com um padrão de fractal preto e branco
ImageProcessing::ImageProcessing(int largura, int altura, string modo){

    int r, g, b;

    if(modo != "random" && modo != "black" && modo != "white" && modo != "fractal"){
        cout << "ImageProcessing: Modo " + modo + " inválido." << endl;
        return;
    }

    this->path = "None";
    this->largura = largura;
    this->altura = altura;

    if(modo == "random"){
        for(int i = 0; i < this->altura; i++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < this->largura; j++){
                r = rand() % 255;
                g = rand() % 255;
                b = rand() % 255;
                linha->push_back(new Pixel(r, g, b));
            }
            this->pixels.push_back(*linha);
        }
    }
    else if(modo == "fractal"){
        for(int i = 0; i < this->altura; i++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < this->largura; j++){
                r = (i * j) % 255;
                g = (i * j) % 255;
                b = (i * j) % 255;
                linha->push_back(new Pixel(r, g, b));
            }
            this->pixels.push_back(*linha);
        }
    }
    else if(modo == "black"){
        for(int i = 0; i < this->altura; i++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < this->largura; j++){
                linha->push_back(new Pixel(0, 0, 0));
            }
            this->pixels.push_back(*linha);
        }
    }
    else if(modo == "white"){
        for(int i = 0; i < this->altura; i++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < this->largura; j++){
                linha->push_back(new Pixel(255, 255, 255));
            }
            this->pixels.push_back(*linha);
        }
    }

    this->MagickImage = Image(Geometry(this->largura, this->altura), ColorRGB(0, 0, 0));
    for(int i = 0; i < this->altura; i++){
        for(int j = 0; j < this->largura; j++){
            this->MagickImage.pixelColor(j, i, ColorRGB(this->pixels[i][j]->getRed(), this->pixels[i][j]->getGreen(), this->pixels[i][j]->getBlue()));
        }
    }
}

// @brief: destrutor
ImageProcessing::~ImageProcessing(){    
    for(auto linha : this->pixels){
        for(auto pixel : linha){
            delete pixel;
        }
    }
}

// getters
string ImageProcessing::getPath(){
    if(this->path == "None"){
        return "Imagem não possui caminho.";
    }
    return this->path;
}

int ImageProcessing::getAltura(){
    return this->altura;
}

int ImageProcessing::getLargura(){
    return this->largura;
}

void ImageProcessing::getPixels(){
    for(auto linha : this->pixels){
        for(auto pixel : linha){
            cout << pixel->getRed() << " " << pixel->getGreen() << " " << pixel->getBlue() << endl;
        }
    }
}

// setters

// operações com imagens em C++
// @brief: redimensiona a imagem, após isso atualiza o vetor de pixels da classe
int ImageProcessing::resize(int new_largura, int new_altura){

    try{
        this->MagickImage.resize(Geometry(new_largura, new_altura));
    }
    catch(Error &error){
        cout << "Erro ao redimensionar a imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }

    this->largura = new_largura;
    this->altura = new_altura;
    
    // refaz o vetor de pixels da classe
    if(!this->ppmToVector()){
        cout << "Erro ao refazer vetor de pixels da imagem redimensionada." << endl;
        return 0;;
    }

    return 1;
}

// operações com imagens das funções em python

// funções auxiliares
// @brief: lê um arquivo ppm e transforma em um vetor de pixels da classe
int ImageProcessing::ppmToVector(){

    srand(time(NULL));
    char temp[20];

    // converte a Magick::Image para um arquivo ppm para pegar os pixels
    try{
        for(int i = 0; i < 20; i++){
            temp[i] = rand() % 26 + 97;
        }
        this->MagickImage.write("./tmp/" + string(temp) + ".ppm");   
    }
    catch(...){
        cout << "Erro ao converter a imagem para um arquivo ppm." << endl;
        return 0;
    }
    
    // le o arquivo ppm e constroi o vetor de pixels
    ifstream ppm_image("./tmp/" + string(temp) + ".ppm");
    
    if(!ppm_image.is_open() || ppm_image.eof() || ppm_image.fail() || ppm_image.bad()){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        return 0;
    }

    string _file_identifier;
    ppm_image >> _file_identifier;
    if(_file_identifier != "P6"){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        return 0;
    }

    int _altura, _largura;
    ppm_image >> _largura >> _altura;
    ppm_image >> this->max_color;

    if(_altura != this->altura || _largura != this->largura){
        cout << "Aviso: O arquivo PPM não possui as dimensões de altura e largura da imagem de entrada. Isso pode ocasionar erros." << endl;
    }

    int r, g, b;
    for(int i = 0; i < _altura; i ++){
        vector<Pixel *> *linha = new vector<Pixel *>();
        for(int j = 0; j < _largura; j++){
            if(!ppm_image.eof()){
                ppm_image >> r >> g >> b;
                linha->push_back(new Pixel(r, g, b));
            }
            else{
                delete linha;
                break;
            }
        }
        this->pixels.push_back(*linha);
    }

    ppm_image.close();

    try{
        remove(("./tmp/" + this->path + ".ppm").c_str());
    }
    catch(...){
        cout << "Erro ao remover o arquivo temporário." << endl;
        return 0;
    }

    return 1;
}

// @brief: transforma o vetor de pixels da classe em um numpy array
int ImageProcessing::vectorToArray(){
    // to do
    return 1;
}

// @brief: salva a imagem com o nome e o formato especificados
int ImageProcessing::save(string output_path){

    try{
        if(output_path == "" || output_path == " " || this->MagickImage == NULL) throw;

        this->MagickImage.write(output_path);
    }
    catch(Magick::Error &error){
        cout << error.what() << endl;
        cout << "Erro ao salvar a imagem. Verifique se o formato é válido." << endl;
        return 0;
    }
    catch(...){
        cout << "Erro ao salvar a imagem. Verifique se o formato é válido." << endl;
        return 0;
    }

    return 1;
}


// operadores

// @brief: sobrecarga do operador de soma. Soma um filtro a todos os pixels da imagem.
ImageProcessing& ImageProcessing::operator+=(Pixel& filtro_pixel){

    // to do: fazer o loop eficiente
    for(auto linha : this->pixels){
        for(auto pixel : linha){
            *pixel = *pixel + filtro_pixel;
        }
    }

    return *this;
}

// @brief: sobrecarga do operador de multiplicação. Faz a convolução de uma matriz filtro com a imagem.


// @brief: sobrecarga do operador []. Retorna um vetor de pixels da linha especificada.
vector<Pixel *>& ImageProcessing::operator[](int index){
    return this->pixels[index];
}