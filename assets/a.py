from PIL import Image
import numpy as np

def cubemap_to_faces(cubemap_image_path, output_dir):
    # Cargar la imagen del cubemap en formato PNG
    cubemap = Image.open(cubemap_image_path)
    
    # Convertirla a un arreglo numpy
    cubemap_array = np.array(cubemap)
    
    # Asumir que la imagen es un cubemap con 6 caras (en una cuadrícula 3x4)
    width, height = cubemap.size
    face_size_x = width // 4  # Dividir en 4 para la dimensión horizontal
    face_size_y = height // 3  # Dividir en 3 para la dimensión vertical
    
    # Coordenadas de las caras del cubemap en la cuadrícula 3x4
    faces = [
        (1, 3),  # Cara derecha
        (1, 1),  # Cara izquierda
        (0, 1),  # Cara arriba
        (2, 1),  # Cara abajo
        (1, 2),  # Cara adelante
        (1, 0),  # Cara atrás
    ]
    
    # Recortar y guardar cada cara
    for i, (row, col) in enumerate(faces):
        # Calcular la posición de la cara en la cuadrícula
        left = col * face_size_x
        upper = row * face_size_y
        right = left + face_size_x
        lower = upper + face_size_y
        
        # Recortar la cara
        face = cubemap_array[upper:lower, left:right]
        
        # Convertir la cara recortada a imagen y guardarla
        face_image = Image.fromarray(face)
        face_image.save(f"{output_dir}/face_{i}.png")  # Guardar como PNG

# Uso del script
cubemap_to_faces("./sky.png", ".")


