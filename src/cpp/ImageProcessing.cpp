#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "ImageProcessing.h"

// construtores e destrutores
// @brief: construtor padrão, gera um objeto nulo.
ImageProcessing::ImageProcessing() {
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

// @brief: construtor que recebe um vetor de pixels e as dimensões da imagem. A imagem saída será totalmente preenchida com o vetor de pixels
ImageProcessing::ImageProcessing(vector<Pixel> &std_value, int largura, int altura){
    this->path = "None";
    this->largura = largura;
    this->altura = altura;

    if((unsigned) largura*altura != std_value.size()){
        cout << "Erro durante a criação da classe ImageProcessing: dimensões da imagem e do vetor de pixels não batem." << endl;
        return;
    }

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

// @brief: construtor de cópia
ImageProcessing::ImageProcessing(ImageProcessing &source){
    this->path = source.path;
    this->largura = source.largura;
    this->altura = source.altura;
    this->max_color = source.max_color;
    this->pixels = source.pixels;
    this->MagickImage = source.MagickImage;
}

// @brief: destrutor
ImageProcessing::~ImageProcessing(){ 
    // to do
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

// @brief: comprime a imagem, após isso atualiza o vetor de pixels da classe
int ImageProcessing::compress(size_t nivel){

    if(nivel > 100 || nivel <= 0){
        cout << "Nível de compressão inválido." << endl;
        return 0;
    }

    try{
        this->MagickImage.quality(nivel);
    }
    catch(Error &error){
        cout << "Erro ao comprimir a imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao gerar a imagem." << endl;
        return 0;
    }

    if(!this->ppmToVector()){
        cout << "Erro ao refazer vetor de pixels da imagem comprimida." << endl;
        return 0;
    }

    return 1;
}

int ImageProcessing::filter(int filter_type, const ImageProcessing* kernel_image){
    return 0;
}

// @brief: converte a imagem para tons de cinza e atualiza o objeto ImageMagick
int ImageProcessing::grayscale(){
    try{
        this->MagickImage.type(GrayscaleType);
    }
    catch(Error &error){
        cout << "Erro ao converter a imagem para tons de cinza." << endl;
        cout << error.what() << endl;
        return 0;
    }

    if(!this->ppmToVector()){
        cout << "Erro ao refazer vetor de pixels da imagem em tons de cinza." << endl;
        return 0;
    }

    return 1;
}

int ImageProcessing::convert(string output_path, string format){
    return 0;
}

// operações com imagens das funções em python

int ImageProcessing::mirror(bool horizontal){
    return 0;
}

int ImageProcessing::rotate(float angle){
    return 0;
}

int ImageProcessing::to_ASCII(){
    return 0;
}

int ImageProcessing::to_pixel(int pixel_size){
    return 0;
}

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
    ifstream ppm_image("./tmp/" + string(temp) + ".ppm", ios::binary);
    
    if(!ppm_image.is_open() || ppm_image.eof() || ppm_image.fail() || ppm_image.bad()){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        return 0;
    }

    eat_comment(ppm_image);
    int mode;

    string _file_identifier;
    ppm_image >> _file_identifier;
    
    if(_file_identifier != "P6" && _file_identifier != "P3"){
        cout << "Erro ao abrir o arquivo ppm. Verifique se o arquivo está corrompido." << endl;
        ppm_image.close();
        return 0;
    }

    if(_file_identifier == "P6"){
        mode = 6;
    } else mode = 3;

    int _altura, _largura;

    eat_comment(ppm_image);
    ppm_image >> _largura;

    eat_comment(ppm_image);
    ppm_image >> _altura;

    eat_comment(ppm_image);
    ppm_image >> this->max_color;

    if(_altura != this->altura || _largura != this->largura){
        cout << "Aviso: O arquivo PPM não possui as dimensões de altura e largura da imagem de entrada. Isso pode ocasionar erros." << endl;
        this->altura = _altura;
        this->largura = _largura;
    }

    if(max_color < 1){
        cout << "Erro ao abrir o arquivo ppm. Número de bits não suportado." << endl;
        ppm_image.close();
        return 0;
    }

    if(_altura < 1 || _largura < 1){
        cout << "Dimensões da imagem não suportadas. Verifique o arquivo por erros." << endl;
        ppm_image.close();
        return 0;
    }

    int r, g, b;

    if(mode == 6){
        vector<RGB> _temp_data;
        
        _temp_data.resize(_altura * _largura);
        ppm_image.get();
        ppm_image.read((char *) &_temp_data[0], _altura * _largura * 3);


        for(int i = 0; i < _altura; i++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < _largura; j++){
                r = (int) _temp_data[i*_largura + j].r;
                g = (int) _temp_data[i*_largura + j].g;
                b = (int) _temp_data[i*_largura + j].b;
                linha->push_back(new Pixel(r, g, b));
            }

            this->pixels.push_back(*linha);
        }
    }else if(mode == 3){
        for(int i = 0; i < _altura; i ++){
            vector<Pixel *> *linha = new vector<Pixel *>();
            for(int j = 0; j < _largura; j++){
                if(!ppm_image.eof()){
                    ppm_image >> r;
                    ppm_image >> g;
                    ppm_image >> b;
                    r = (int) 255*r/this->max_color;
                    g = (int) 255*g/this->max_color;
                    b = (int) 255*b/this->max_color;
                    linha->push_back(new Pixel(r, g, b));
                }
                else{
                    delete linha;
                    break;
                }
            }
            this->pixels.push_back(*linha);
        }
    }

    ppm_image.close();

    try{
        remove(("./tmp/" + string(temp) + ".ppm").c_str());
    }
    catch(...){
        cout << "Erro ao remover o arquivo temporário." << endl;
        return 0;
    }

    cout << "Done" << endl;
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
        if(output_path == "") throw 1;

        this->MagickImage.write(output_path);
    }
    catch(Magick::Error &error){
        cout << error.what() << endl;
        cout << "Erro ao salvar a imagem. Verifique se o formato é válido." << endl;
        return 0;
    }
    catch(...){
        cout << "Erro ao salvar a imagem. Verifique se o path de saída foi inserido corretamente." << endl;
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
ImageProcessing& ImageProcessing::operator*(ImageProcessing& kernel_matrix){

    // to do: fazer o loop eficiente
    ImageProcessing *_convolved_image = new ImageProcessing;
    for(int i = 0; i < this->altura; i++){
        for(int j = 0; j < this->largura; j++){
            // cria um pixel preto
            Pixel *_pixel = new Pixel(0, 0, 0);
            for(int k = 0; k < kernel_matrix.getAltura(); k++){
                for(int l = 0; l < kernel_matrix.getLargura(); l++){
                    int _pixel_x = j + l - (kernel_matrix.getLargura() / 2);
                    int _pixel_y = i + k - (kernel_matrix.getAltura() / 2);
                    if(_pixel_x >= 0 && _pixel_x < this->largura && _pixel_y >= 0 && _pixel_y < this->altura){
                        *_pixel += *this->pixels[_pixel_y][_pixel_x] * *kernel_matrix[k][l];
                    }
                }
            }
            _convolved_image->pixels[i].push_back(_pixel);
        }
    }

    return *_convolved_image;
}


// @brief: sobrecarga do operador []. Retorna um vetor de pixels da linha especificada.
vector<Pixel *>& ImageProcessing::operator[](int index){
    return this->pixels[index];
}

// funções externas a classe

void eat_comment(ifstream &f)
{
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}