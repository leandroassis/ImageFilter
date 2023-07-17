import numpy as np
from PIL import Image, ImageOps

def mirror_image(img_array : np.ndarray, output_path : str, vertical_or_horizontal : int = 1) -> int:

    try:
        if vertical_or_horizontal != 1 and vertical_or_horizontal != 2:
            raise ValueError("O parâmetro vertical_or_horizontal deve ser 1 ou 2.")
        
        image = Image.fromarray(img_array) # converte o np.array para imagem do PIL

        image = ImageOps.mirror(image) if vertical_or_horizontal == 1 else ImageOps.flip(image)

        image.save("./out/"+output_path) # salva a imagem modificada

    except Exception as e:
        print(f"Erro: {e}.")
        print("Erro ao espelhar a imagem.")
        return 0
    else:
        return 1
    
if __name__ == "__main__":

    img_path = "./sample/CristoRedentor-4.jpeg"
    img = Image.open(img_path)
    img_array = np.array(img)

    mirror_image(img_array, "Cristo_mirrored.png", 1)
