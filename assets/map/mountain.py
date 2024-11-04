import csv
import random

# Parámetros del terreno
ancho = 20
largo = 100
separacion = 5
max_variacion_y = 1   # Variación máxima en el eje Y
pendiente_y = 0.1     # Factor de inclinación en el eje Y

# Nombre del archivo CSV
archivo_csv = 'terreno_montana.csv'

# Generación del terreno
with open(archivo_csv, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(['x', 'y', 'z'])  # Cabecera del archivo CSV
    
    # Bucle para generar vértices
    for z in range(0, largo + 1, separacion):
        for x in range(0, ancho + 1, separacion):
            # Cálculo de la posición y con inclinación y variación
            y = -pendiente_y * z + random.uniform(-max_variacion_y, max_variacion_y)
            writer.writerow([x, y, z])

print(f"Archivo {archivo_csv} generado exitosamente con los vértices del terreno.")