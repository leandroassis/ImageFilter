## ImageProcessing: C++ Image Processing Library

### Instalação

Para instalar e executar o programa é necessário ter o ImageMagick++7 instalado e Python (preferencialmente v3.10).

Caso seja necessário utilizar outra versão do Python, altere, no `Makefile`, as variáveis PYTHON_LIB para `python(versão)` e PYTHON_INCLUDE para o path de instalação dos arquivos da versão do Python.

Para instalar as bibliotecas do python necessárias, execute `pip install -r requirements.txt` na pasta raiz do projeto.

Siga os passos a seguir para instalar o ImageMagick++7:

```
sudo apt-get install -y libjpeg62-dev libjpg-dev libpng-dev

sudo apt-get install -y libtiff-dev 

git submodule update --init 

cd ImageMagick/[version_number] # atualmente 7.1.0

./configure --disable-shared

sudo make

sudo make install

sudo make check
```

### Compilação
Para compilar o projeto, execute `make` na pasta raiz do projeto. Isso irá gerar o arquivo executável `ImageProcessing` na raiz do projeto.

Para a compilação são utilizados os arquivos `main.cpp` e `ImageProcessing.cpp` e os arquivos de cabeçalho `ImageProcessing.h` e `ImageProcessingUtils.h`. Além disso são utilizados os .h do ImageMagick++7 e do Python, certifique-se de que eles estão instalados e o path é o descrito no `Makefile`.

Para limpar os arquivos gerados pela compilação, execute `make clean`. Isso também irá limpar a pasta `tmp/`.

### Execução

O programa foi projetado para ser executado através de linha de comando. As interações do usuário são feitas através de argumentos passados na linha de comando.

As funções e respectivos argumentos são:

| short option | Requisitos | Operação |
| -m | 1 (horizontal) ou 0 (vertical) e -s | Espelha a imagem |
| -r | ângulo -s | Rotaciona a imagem em `ângulo` graus |
| -s | path de saída | Caminho para salvar a imagem de saída |
| -c | formato -s | Converte a imagem para outro formato |
| -g | None | Converte a imagem para escala de cinza |
| -n | None | Converte a imagem para negativo |
| -i | path de entrada | Caminho da imagem de entrada |
| -d | None | Remove ruído da imagem usando um filtro gaussiano |
| -b | raio do blur = 1 (opcional) | Adiciona blur na imagem |
| -l | None | Adiciona ruído gaussiano a imagem |
| -u | -w largura -h altura | Redimensiona a imagem |
| -w | largura | Configura a largura para o redimensionamento |
| -h | altura | Configura a altura para o redimensionamento |
| -p | nº de pixels = 16 -s | Transforma a imagem em pixel art |
| -a |-s | Transforma a imagem em ASCII art e salva em um arquivo `.txt` |
| -e | nível de compressão | Comprime a imagem reduzindo sua qualidade |

### Referências Bibliográficas

[ImageMagick++ library introduction](https://imagemagick.org/Magick++/tutorial/Magick++_tutorial.pdf)
[Magick++ API docs](https://www.imagemagick.org/Magick++/Image++.html)