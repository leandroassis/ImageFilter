import numpy as np
from PIL import Image

def rotate_image(img_array : list, output_path : str, angle : float, expand : bool  = True) -> int:

    img_array = np.array(img_array) # converte a imagem para np.array

    try:
        image = Image.fromarray(img_array.astype(np.uint8)) # converte o np.array para imagem do PIL

        image = image.rotate(angle, expand = expand)
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

    rotate_image(img_array, "Cristo_rotated.png", 90)
