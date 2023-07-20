to do:

- progress bar \
- menu \
- tratamento de erro \
- otimização de processamento


Funções em C++

- redimensionar imagem \
- Comprimir imagem jpeg para tiff\
- Aplicar filtros na imagem \
- Converter entre arquivos

### Instalação

Execute `pip install -r requirements.txt` para instalar as dependências do projeto. 

Siga os passos a seguir para instalar o ImageMagick++7:

```sudo apt-get install -y libjpeg62-dev libjpg-dev libpng-dev
sudo apt-get install -y libtiff-dev 

git submodule update --init 

cd ImageMagick/[version_number] # atualmente 7.1.0

./configure --disable-shared

sudo make

sudo make install

sudo make check```

### Referências Bibliográficas

[ImageMagick++ library introduction](https://imagemagick.org/Magick++/tutorial/Magick++_tutorial.pdf)
[Magick++ API docs](https://www.imagemagick.org/Magick++/Image++.html)