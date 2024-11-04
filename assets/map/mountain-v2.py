import csv
import random

# Parámetros del terreno
ancho = 4
largo = 10
separacion = 5
max_variacion_y = 0.5    # Variación máxima en el eje Y para el terreno
pendiente_y = 0.05      # Factor de inclinación en el eje Y para el descenso
altura_acera = 3       # Altura de las aceras a los lados de la pista

# Nombre del archivo CSV
archivo_csv = 'pista_con_acera.csv'

# Generación de la pista con acera
with open(archivo_csv, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['x', 'y', 'z'])  # Cabecera del archivo CSV
    
    # Bucle para generar vértices de la pista y las aceras
    for z in range(0, largo + 1, separacion):
        # Acera izquierda
        x_left = -separacion/3  # Posición de la acera izquierda fuera de la pista
        y_left = -pendiente_y * z + altura_acera  # Altura de la acera
        writer.writerow([x_left, y_left, z])

        # Vértices de la pista
        for x in range(0, ancho + 1, separacion):
            # Cálculo de la posición y con inclinación y variación
            y = -pendiente_y * z + random.uniform(-max_variacion_y, max_variacion_y)
            writer.writerow([x, y, z])
        
        # Acera derecha
        x_right = ancho + separacion/3  # Posición de la acera derecha fuera de la pista
        y_right = -pendiente_y * z + altura_acera  # Altura de la acera
        writer.writerow([x_right, y_right, z])

print(f"Archivo {archivo_csv} generado exitosamente con la pista y aceras laterales.")
