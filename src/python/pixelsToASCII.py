from PIL import Image
import numpy as np

ASCII_CHARS = '@%#*$&[]}{^+=-:. '

# recebe uma imagem do PIL e retorna uma string com os caracteres ASCII
def map_pixels_to_ascii(image : Image) -> str:
    # adicionar recursão
    pixels = image.getdata()
    ascii_str = ''

    for pixel_value in pixels:
        gray = int(0.299*pixel_value[0] + 0.587*pixel_value[1] + 0.114*pixel_value[2]) # calcula o valor de cinza do pixel
        ascii_str += ASCII_CHARS[gray // len(ASCII_CHARS)] # mapeia o valor de cinza para um caractere ASCII
    return ascii_str

# recebe uma imagem como np.array e converte para ascii, salvando no arquivo apontado pelo path
def convert_image_to_ascii(image : list, output_path : str) -> int:
    image = np.array(image)

    try:
        pil_image = Image.fromarray(image.astype(np.uint8)) # converte o np.array para imagem do PIL

        ascii_str = map_pixels_to_ascii(pil_image) # converte a imagem para ascii
        
        img_width = pil_image.width # largura da imagem
        ascii_str_len = len(ascii_str) # tamanho da string ascii
        
        with open(output_path, 'w') as f:
            for i in range(0, ascii_str_len, img_width):
                f.write(ascii_str[i:i+img_width] + '\n')
    except Exception as e:
        print(f'Erro: {e}.')
        print('Erro ao converter a imagem para ASCII.')
        return 0
    else:
        return 1

if __name__ == '__main__':
    slice56 = Image.open('./sample/CristoRedentor-4.jpeg')
    slice56 = np.array(slice56)

    print(convert_image_to_ascii(slice56, 'Cristo_ASCII.txt'))
