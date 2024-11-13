import csv
import math

# Parámetros del terreno
ancho = 4
largo = 120
separacion = 5
pendiente_y = 0.2
altura_acera = 6  
amplitud_curva = 4  
frecuencia_curva = 0.4  

# Parámetros para la función gaussiana
altura_gaussiana1 = 0.8
altura_gaussiana2 = 0.4
sigma_x1, sigma_z1 = 6.0, 8.0
sigma_x2, sigma_z2 = 2.5, 2.0
frecuencia_gaussiana = 1  # Nueva frecuencia para la variación sinusoidal

archivo_csv = 'map.csv'

# Función para calcular la altura en y basada en la suma de varias gaussianas
def gaussian_height(x, z):
    # Introduce una variación sinusoidal en z para simular una frecuencia
    z_variation = math.sin(frecuencia_gaussiana * z)
    gaussian1 = altura_gaussiana1 * math.exp(-((x - ancho / 2) ** 2) / (2 * sigma_x1 ** 2)) * math.exp(-((z - largo / 2 + z_variation) ** 2) / (2 * sigma_z1 ** 2))
    gaussian2 = altura_gaussiana2 * math.exp(-((x - ancho / 2) ** 2) / (2 * sigma_x2 ** 2)) * math.exp(-((z - largo / 4 + z_variation) ** 2) / (2 * sigma_z2 ** 2))
    y = gaussian1 + gaussian2 - pendiente_y * z
    return y

# Generación de la pista con acera
with open(archivo_csv, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['x', 'y', 'z'])
    
    for z in range(0, largo + 1):
        desplazamiento_x = amplitud_curva * math.sin(frecuencia_curva * z)

        # Acera izquierda
        x_left = -separacion // 3 + desplazamiento_x
        y_left = -pendiente_y * z + altura_acera
        writer.writerow([x_left, y_left, separacion * z])

        # Vértices de la pista
        for x in range(0, ancho + 1):
            pos_x = x + desplazamiento_x + separacion * x
            y = gaussian_height(pos_x, z)
            writer.writerow([pos_x, y, separacion * z])
        
        # Acera derecha
        x_right = ancho + separacion // 3 + desplazamiento_x + separacion * x
        y_right = -pendiente_y * z + altura_acera
        writer.writerow([x_right, y_right, separacion * z])

print(f"Archivo {archivo_csv} generado exitosamente con la pista y aceras laterales curvadas.")
