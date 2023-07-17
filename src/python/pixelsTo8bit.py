import numpy as np
from PIL import Image

def convert_to_8bit_art(img_array : np.ndarray, output_path : str, pixel_size : int = 16) -> int:

    try:
        image = Image.fromarray(img_array) # converte o np.array para imagem do PIL

        image = image.resize(
            (image.size[0] // pixel_size, image.size[1] // pixel_size),
            Image.NEAREST
        )
        image = image.resize(
            (image.size[0] * pixel_size, image.size[1] * pixel_size),
            Image.NEAREST
        )

        image.save("./out/"+output_path) # salva a imagem modificada
    except Exception as e:
        print(f"Erro: {e}.")
        print("Erro ao rotacionar a imagem.")
        return 0
    else:
        return 1
    
if __name__ == "__main__":

    img_path = "./sample/CristoRedentor-4.jpeg"
    img = Image.open(img_path)
    img_array = np.array(img)

    convert_to_8bit_art(img_array, "Cristo_8bits.png", 16)
