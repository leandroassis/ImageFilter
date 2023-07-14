from PIL import Image

ASCII_CHARS = '@%#*+=-:. '

def map_pixels_to_ascii(image, range_width=25):
    pixels = image.getdata()
    ascii_str = ''
    for pixel_value in pixels:
        ascii_str += ASCII_CHARS[pixel_value // range_width]
    return ascii_str

def convert_image_to_ascii(image):
    ascii_str = map_pixels_to_ascii(image)
    img_width = image.width
    ascii_str_len = len(ascii_str)
    ascii_img = ''
    for i in range(0, ascii_str_len, img_width):
        ascii_img += ascii_str[i:i+img_width] + '\n'
    return ascii_img

def main(image_path):
    try:
        image = Image.open(image_path)
    except Exception as e:
        print(e)
        return
    ascii_img = convert_image_to_ascii(image)
    print(ascii_img)

# Exemplo de uso:
main('caminho_da_imagem.jpg')
