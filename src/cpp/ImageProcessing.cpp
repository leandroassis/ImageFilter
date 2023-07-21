#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

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
        ifstream _tempfile(this->path);
        if(!_tempfile){
            cout << "Erro durante a criação da classe ImageProcessing: caminho da imagem inválido." << endl;
            exit(1);
        }
        
        this->MagickImage.read(this->path);
        _tempfile.close();
    } 
    catch (Magick::ErrorFileOpen &error) {
        cout << "Erro durante a criação da classe ImageProcessing: " << error.what() << endl;
        cout << "Verifique se o caminho da imagem está correto." << endl;
        exit(1);
    }
    catch (Magick::ErrorCorruptImage &error){
        cout << "Erro durante a criação da classe ImageProcessing: " << error.what() << endl;
        cout << "Verifique se a imagem está corrompida." << endl;
        exit(1);
    }
    catch (Magick::ErrorMissingDelegate &error){
        cout << "Erro durante a criação da classe ImageProcessing: " << error.what() << endl;
        cout << "Verifique se o formato da imagem é suportado." << endl;
        exit(1);
    }
    catch (...){
        cout << "Erro desconhecido durante a criação da classe ImageProcessing." << endl;
        exit(1);
    }

    // pega os atributos da imagem de entrada
    this->altura = this->MagickImage.rows();
    this->largura = this->MagickImage.columns();

    // le todos os pixels da imagem e constroi o vetor de pixels
    if(!this->ppmToVector()){
        cout << "Erro ao gerar vetor de pixels da imagem." << endl;
        exit(1);
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

// @brief: converte a imagem para tons de cinza e atualiza o objeto ImageMagick
int ImageProcessing::grayscale(){
    try{
        this->MagickImage.type(Magick::GrayscaleType);
        // mudar para grayscale manualmente
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

// @brief: converte a imagem para um outro formato e salva no path especificado
int ImageProcessing::convert(string output_path, string format){
    if(format != "jpg" && format != "png" && format != "bmp" && format != "tiff" && format != "gif" && format != "ppm"){
        cout << "Tentando converter imagem para formato inválido." << endl;
        return 0;
    }

    if(output_path == ""){
        cout << "Tentando converter imagem para path de saída inválido." << endl;
        return 0;
    }

    // se a extensão (últimos format.size() dígitos) não for igual a format, retorna erro
    if(output_path.substr(output_path.size() - format.size()).find(format) == std::string::npos){
        cout << "A extensão do arquivo de saída não é compatível com o formato escolhido para conversão." << endl;
        return 0;
    }

    try{
        this->MagickImage.write(output_path);
    }
    catch(ErrorMissingDelegate &error){
        cout << "Erro ao converter a imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao converter a imagem." << endl;
        return 0;
    }


    return 1;
}

// @brief: adiciona ruído à imagem
int ImageProcessing::noise(){
    try{
        this->MagickImage.addNoise(GaussianNoise);
    }
    catch(Error &error){
        cout << "Erro ao adicionar ruído à imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao adicionar ruído à imagem." << endl;
        return 0;
    }

    return 1;
}

// @brief: aplica um filtro de blur gaussiano à imagem usando o desvio padrão especificado e um kernel de 3x3
int ImageProcessing::blur(const double std){
    try{
        this->MagickImage.gaussianBlur(1, std);
    }
    catch(Error &error){
        cout << "Erro ao borrar a imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao borrar a imagem." << endl;
        return 0;
    }

    return 1;
}

// @brief: aplica um filtro de redução de ruído à imagem
int ImageProcessing::denoise(){
    try{
        this->MagickImage.reduceNoise();
    }
    catch(Error &error){
        cout << "Erro ao reduzir ruído da imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao reduzir ruído da imagem." << endl;
        return 0;
    }

    return 1;
}

int ImageProcessing::negative(){
    try{
        this->MagickImage.negate();
    }
    catch(Error &error){
        cout << "Erro ao negativar a imagem." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro desconhecido ao negativar a imagem." << endl;
        return 0;
    }

    return 1;
}

// operações com imagens das funções em python

// @brief: espelha a imagem horizontalmente ou verticalmente
int ImageProcessing::mirror(int horizontal, string out_path){

    if(horizontal != 0 && horizontal != 1){
        cout << "Erro ao espelhar a imagem. Modo inválido." << endl;
        return 0;
    }

    if(out_path == ""){
        cout << "Erro ao espelhar a imagem. Imagem não possui caminho de saída." << endl;
        return 0;
    }

    PyObject *pName, *pModule, *pFunc, *pArgs, *pCall;
    
    pName = PyUnicode_FromString("src.python.mirrorImage");
    pModule = PyImport_Import(pName);

    if(!pModule){
        cout << "Erro ao importar o módulo mirrorImage." << endl;
        return 0;
    }

    pFunc = PyObject_GetAttrString(pModule, (char *)"mirror_image");

    this->vectorToArray();

    pArgs = PyTuple_Pack(3, pList, PyUnicode_FromString(out_path.c_str()), PyBool_FromLong(horizontal));
    pCall = PyObject_CallObject(pFunc, pArgs);

    return PyLong_AsLong(pCall);
}

// @brief: rotaciona a imagem em um ângulo especificado
int ImageProcessing::rotate(float angle, int expand, string out_path){

    if(angle > 360 || angle < -360){
        cout << "Erro ao rotacionar a imagem. Ângulo inválido." << endl;
        return 0;
    }

    if(out_path == ""){
        cout << "Erro ao espelhar a imagem. Imagem não possui caminho de saída." << endl;
        return 0;
    }

    PyObject *pName, *pModule, *pFunc, *pArgs, *pCall;
    
    pName = PyUnicode_FromString("src.python.rotateImage");
    pModule = PyImport_Import(pName);

    if(!pModule){
        cout << "Erro ao importar o módulo rotateImage." << endl;
        return 0;
    }

    pFunc = PyObject_GetAttrString(pModule, (char *)"rotate_image");

    this->vectorToArray();

    pArgs = PyTuple_Pack(4, pList, PyUnicode_FromString(out_path.c_str()), PyFloat_FromDouble((double) angle), PyBool_FromLong(expand));
    pCall = PyObject_CallObject(pFunc, pArgs);

    return PyLong_AsLong(pCall);
}

// @brief: converte a imagem para ASCII e salva no path especificado
int ImageProcessing::to_ASCII(string out_path){
    
    if(out_path == ""){
        cout << "Erro ao converter a imagem para ASCII. Imagem não possui caminho de saída." << endl;
        return 0;
    }

    PyObject *pName, *pModule, *pFunc, *pArgs, *pCall;
    
    pName = PyUnicode_FromString("src.python.pixelsToASCII");
    pModule = PyImport_Import(pName);

    if(!pModule){
        cout << "Erro ao importar o módulo pixelsToASCII." << endl;
        return 0;
    }

    pFunc = PyObject_GetAttrString(pModule, (char *)"convert_image_to_ascii");

    this->vectorToArray();

    pArgs = PyTuple_Pack(2, pList, PyUnicode_FromString(out_path.c_str()));
    pCall = PyObject_CallObject(pFunc, pArgs);

    return PyLong_AsLong(pCall);
}

// @brief: converte a imagem para um numpy array
int ImageProcessing::to_pixel(string out_path, int pixel_size){
    if(pixel_size <= 0 || pixel_size > this->altura*this->largura){
        cout << "Erro ao converter a imagem para pixel. Tamanho do pixel inválido." << endl;
        return 0;
    }

    if(out_path == ""){
        cout << "Erro ao converter a imagem para pixel. Imagem não possui caminho de saída." << endl;
        return 0;
    }

    PyObject *pName, *pModule, *pFunc, *pArgs, *pCall;
    
    pName = PyUnicode_FromString("src.python.pixelsTo8bit");
    pModule = PyImport_Import(pName);

    if(!pModule){
        cout << "Erro ao importar o módulo pixelsTo8bit." << endl;
        return 0;
    }

    pFunc = PyObject_GetAttrString(pModule, (char *)"convert_to_8bit_art");

    this->vectorToArray();

    pArgs = PyTuple_Pack(3, pList, PyUnicode_FromString(out_path.c_str()), PyLong_FromLong(pixel_size));
    pCall = PyObject_CallObject(pFunc, pArgs);

    return PyLong_AsLong(pCall);
}

// funções auxiliares
// @brief: lê um arquivo ppm e transforma em um vetor de pixels da classe
int ImageProcessing::ppmToVector(){

    char temp[5];

    // converte a Magick::Image para um arquivo ppm para pegar os pixels
    try{
        for(int i = 0; i < 5; i++){
            temp[i] = rand() % 25 + 97;
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

    return 1;
}

// @brief: transforma o vetor de pixels da classe em um numpy array
int ImageProcessing::vectorToArray(){
    try{
        this->pList = PyList_New(this->altura);

        for(int i = 0; i < this->altura; i++){
            PyObject *pList2 = PyList_New(this->largura);
            for(int j = 0; j < this->largura; j++){
                PyObject *pList3 = PyList_New(3);
                PyList_SetItem(pList3, 0, PyLong_FromLong(this->pixels[i][j]->getRed()));
                PyList_SetItem(pList3, 1, PyLong_FromLong(this->pixels[i][j]->getGreen()));
                PyList_SetItem(pList3, 2, PyLong_FromLong(this->pixels[i][j]->getBlue()));
                PyList_SetItem(pList2, j, pList3);
            }
            PyList_SetItem(this->pList, i, pList2);
        }
    }
    catch(Exception &error){
        cout << "Erro ao transformar o vetor de pixels em um python list." << endl;
        cout << error.what() << endl;
        return 0;
    }
    catch(...){
        cout << "Erro ao transformar o vetor de pixels em um python list." << endl;
        return 0;
    }

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


// @brief: sobrecarga do operador []. Retorna um vetor de pixels da linha especificada.
vector<Pixel *>& ImageProcessing::operator[](int index){
    return this->pixels[index];
}

// funções externas
void eat_comment(ifstream &f)
{
    char linebuf[1024];
    char ppp;
    while (ppp = f.peek(), ppp == '\n' || ppp == '\r')
        f.get();
    if (ppp == '#')
        f.getline(linebuf, 1023);
}