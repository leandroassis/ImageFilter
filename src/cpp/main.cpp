#include <iostream>
#include <Magick++.h>

#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <unistd.h>

#include "ImageProcessing.h"

using namespace std;
using namespace Magick;

int main(int argc, char *argv[]){
    srand(time(NULL));
    setenv("PYTHONPATH",".",1);
    Py_Initialize();

    bool mirror, rotate, to_ASCII, to_pixel, resize, compress, noise, convolve, blur, denoise, remap, negative, grayscale, convert, save;
    mirror = rotate = to_ASCII = to_pixel = resize = compress = noise = convolve = blur = denoise = remap = negative = grayscale = convert = save = false;

    string path(""), output, format, convolve_kernel;
    int altura = 0, largura = 0, hor_vert, num_pixels;
    size_t compress_lvl, convolve_kernel_size;
    float rotate_angle;
    double blur_radius;

    Image imagem_remap;
    ImageProcessing IPKit;

    if(argc < 2){
        cout << "Uso: " << argv[0] << " -i <caminho-da-imagem> [options]" << endl;
        return 1;
    }

    int c;

    // -m 1 ou 0 (mirror)
    // -r angle (rotate)
    // -s outpath
    // -f format
    // -k (remap) precisa passar a imagem com -j
    // -d
    // -g -n -b
    // -c (convolve) -v FAZER 
    // -l noise
    // -i inpath
    // -u (resize) precisa passar altura -h e largura -w
    while((c = getopt(argc, argv, "m:r:s:f:gnkj:i:db:lc:w:h:up:av:e:")) != -1){
        switch(c){
            case 'm':
                mirror = true;
                try{
                    hor_vert = stoi(optarg);
                } catch(...){
                    hor_vert = -1;
                }
                if(hor_vert != 0 && hor_vert != 1){
                    cout << "-m deve ser 0 (vertical) ou 1 (horizontal). O valor será setado ao padrão 1." << endl;
                    hor_vert = 1;
                }
                break;
            case 'r':
                rotate = true;
                try{
                    rotate_angle = stof(optarg);
                } catch(...){
                    rotate_angle = -115.0;
                }
                if(rotate_angle < 0 || rotate_angle > 360){
                    cout << "-r deve ser um valor entre 0 e 360. O valor será setado ao padrão 90." << endl;
                    rotate_angle = 90.0;
                }
                break;
            case 's':
                save = true;
                output = string(optarg);
                break;
            case 'f':
                convert = true;
                format = string(optarg);
                break;
            case 'g':
                grayscale = true;
                break;
            case 'n':
                negative = true;
                break;
            case 'k':
                remap = true;
                break;
            case 'j':
                try{
                    imagem_remap = Image(string(optarg));
                }
                catch(...){
                    cout << "Erro ao abrir imagem para remap. O remap será abortado." << endl;
                    remap = false;
                }
                break;
            case 'i':
                try{
                    path = string(optarg);
                } catch(...){
                    cout << "Erro ao abrir imagem para conversão. O programa será encerrado." << endl;
                    return 1;
                }
                break;
            case 'd':
                denoise = true;
                break;
            case 'b':
                blur = true;
                try{
                    blur_radius = stod(optarg);
                } catch(...){
                    cout << "Erro ao ler raio do blur. O valor será setado ao padrão 1." << endl;
                    blur_radius = 1;
                }
                break;
            case 'l':
                noise = true;
                break;
            case 'u':
                resize = true;
                break;
            case 'w':
                try{
                    largura = stoi(optarg);
                } catch(...){
                    cout << "Erro ao ler largura para resize. O resize não será realizado." << endl;
                    resize = false;
                }
                break;
            case 'h':
                try{
                    altura = stoi(optarg);
                } catch(...){
                    cout << "Erro ao ler altura para resize. O resize não será realizado." << endl;
                    resize = false;
                }
                break;
            case 'p':
                to_pixel = true;
                try{
                    num_pixels = stoi(optarg);
                } catch(...){
                    cout << "Erro ao ler número de pixels para conversão. O valor será setado ao padrão 16." << endl;
                    num_pixels = 16;
                }
                break;
            case 'a':
                to_ASCII = true;
                break;
            case 'c':
                convolve = true;
                try{
                    convolve_kernel_size = stoi(optarg);
                } catch(...){
                    cout << "Erro ao ler tamanho do kernel para convolução. A convolução será abortada." << endl;
                    convolve = false;
                }

                if(convolve_kernel_size < 1){
                    cout << "Matriz de kernel deve ter tamanho maior que 0. A convolução será abortada." << endl;
                    convolve = false;
                }
                break;
            case 'v':
                convolve_kernel = string(optarg);
                break;
            case 'e':
                compress = true;
                try{
                    compress_lvl = (size_t) stoi(optarg);
                } catch(...){
                    cout << "Erro ao ler nível de compressão. O valor será setado ao padrão 50." << endl;
                    compress_lvl = 50;
                }
                break;
            default:
                break;
        }
    }

    if(path != ""){
        cout << "Abrindo imagem..." << endl;
        IPKit = ImageProcessing(path); // cria objeto de processamento de imagem
        cout << "Imagem aberta com sucesso." << endl;
    }
    else{
        cout << "Para converter imagem, é necessário passar o caminho da imagem. Utilize -i <caminho-da-imagem>. O programa será encerrado." << endl;
        return 1;
    }

    // funções do python
    if(mirror){
        if(!save){
            cout << "Para aplicar espelhamento, é necessário salvar a imagem. Utilize -s <path-de-saída>. O programa será encerrado." << endl;
            return 1;
        }
        cout << "Aplicando espelhamento..." << endl;
        if(!IPKit.mirror(hor_vert, output)) cout << "Erro ao aplicar espelhamento." << endl;
        else cout << "Espelhamento aplicado com sucesso." << endl;
    }

    if(rotate){
        if(!save){
            cout << "Para aplicar rotação, é necessário salvar a imagem. Utilize -s <path-de-saída>. O programa será encerrado." << endl;
            return 1;
        }
        cout << "Aplicando rotação..." << endl;
        if(!IPKit.rotate(rotate_angle, 1, output)) cout << "Erro ao aplicar rotação." << endl;
        else cout << "Rotação aplicada com sucesso." << endl;
    }

    if(to_ASCII){
        if(!save){
            cout << "Para converter para ASCII, é necessário salvar a imagem. Utilize -s <path-de-saída>. O programa será encerrado." << endl;
            return 1;
        }
        cout << "Convertendo para ASCII..." << endl;
        if(!IPKit.to_ASCII(output)) cout << "Erro ao converter para ASCII." << endl;
        else cout << "Conversão para ASCII realizada com sucesso." << endl;
    }

    if(to_pixel){
        if(!save){
            cout << "Para converter para pixel art, é necessário salvar a imagem. Utilize -s <path-de-saída>. O programa será encerrado." << endl;
            return 1;
        }
        cout << "Convertendo para pixel..." << endl;
        if(!IPKit.to_pixel(output, num_pixels)) cout << "Erro ao converter para pixel." << endl;
        else cout << "Conversão para pixel realizada com sucesso." << endl;
    }

    // funções do C++

    if(resize){
        if(largura == 0 || altura == 0){
            cout << "Para redimensionar a imagem, é necessário passar altura e largura. Utilize -h <altura> e -w <largura>. O programa será encerrado." << endl;
            return 1;
        }

        cout << "Redimensionando imagem..." << endl;
        if(!IPKit.resize(altura, largura)) cout << "Erro ao redimensionar imagem." << endl;
        else cout << "Imagem redimensionada com sucesso." << endl;
    }

    if(compress){
        if(compress_lvl < 1 || compress_lvl > 100){
            cout << "Nível de compressão deve ser um valor entre 1 e 100. O valor será setado ao padrão 50." << endl;
            compress_lvl = 50;
        }
        cout << "Comprimindo imagem..." << endl;
        if(!IPKit.compress(compress_lvl)) cout << "Erro ao comprimir imagem." << endl;
        else cout << "Imagem comprimida com sucesso." << endl;
    }

    if(noise){
        cout << "Aplicando ruído..." << endl;
        if(!IPKit.noise()) cout << "Erro ao aplicar ruído." << endl;
        else cout << "Ruído aplicado com sucesso." << endl;
    }

    if(convolve){
        if(convolve_kernel == ""){
            cout << "Para aplicar convolução, é necessário passar o kernel. Utilize -v <kernel>. O programa será encerrado." << endl;
            return 1;
        }
        cout << "Aplicando convolução..." << endl;
        if(!IPKit.convolve(convolve_kernel_size, convolve_kernel.c_str())) cout << "Erro ao aplicar convolução." << endl;
        else cout << "Convolução aplicada com sucesso." << endl;
    }

    if(save){
        if(!IPKit.save(output)) cout << "Erro ao salvar imagem." << endl;
        else cout << "Imagem salva com sucesso." << endl;
    }

    Py_Finalize();

    return 0;
}