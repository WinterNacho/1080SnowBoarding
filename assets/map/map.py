import csv
import math

# Parámetros del terreno
ancho = 8  # Número de vértices en la dirección X
largo = 120  # Número de vértices en la dirección Z
separacion_x = 3  # Distancia entre vértices en la dirección X
separacion_z = 10  # Distancia entre vértices en la dirección Z (modificada)
archivo_csv = 'map.csv'

# Parámetros de la función gaussiana
amplitud_gaussiana = 1.2  # Amplitud de la variación de la función gaussiana
sigma = 2  # Desviación estándar de la distribución gaussiana

# Parámetros de la pendiente
pendiente = -0.2  # Cuánto sube o baja el terreno por cada unidad en Z

# Función gaussiana para generar irregularidad en el terreno
def gaussiana(x, z):
    # Centro de la distribución (podemos dejarlo en 0 si queremos una distribución centrada)
    mu = 4.2
    # Función gaussiana para modificar la altura
    return amplitud_gaussiana * math.exp(-((x - mu)**2 + (z - mu)**2) / (2 * sigma**2))

# Función para generar la altura utilizando una combinación de funciones sinusoidales y gaussiana
def altura(x, z):
    # Parámetros para las funciones sinusoidales
    frecuencia_seno_x = 0.15  # Frecuencia de las ondas en la dirección X
    frecuencia_seno_z = 0.04  # Frecuencia de las ondas en la dirección Z
    amplitud_seno_x = 2     # Amplitud de las ondas en la dirección X
    amplitud_seno_z = 1.5     # Amplitud de las ondas en la dirección Z
    
    # Generamos una onda sinusoidal para la dirección X
    onda_seno_x = amplitud_seno_x * math.sin(frecuencia_seno_x * (x + 20))
    
    # Generamos una onda sinusoidal para la dirección Z
    onda_seno_z = amplitud_seno_z * math.cos(frecuencia_seno_z * z)
    
    # Agregamos la irregularidad de la función gaussiana
    irregularidad = gaussiana(x, z)
    
    # Aplicamos la pendiente, la cual varía la altura con base en la coordenada Z
    pendiente_effect = pendiente * z
    
    # La altura final es la combinación de las ondas, la irregularidad gaussiana y la pendiente
    return onda_seno_x + onda_seno_z + irregularidad + pendiente_effect

# Generar la malla y guardar los vértices en el archivo CSV
def generar_malla():
    with open(archivo_csv, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["x", "y", "z"])  # Escribir encabezado
        
        # Generar vértices
        for z in range(largo):  # Primero iterar sobre el eje Z (largo de la malla)
            writer.writerow([-2, altura(3, z * separacion_z) + 10, z * separacion_z ])
            for x in range(ancho):  # Luego iterar sobre el eje X (ancho de la malla)
                # Convertir los índices a las coordenadas espaciales (x, z) multiplicándolos por la separación
                coord_x = x * separacion_x
                coord_z = z * separacion_z  # Aplicamos una separación diferente en Z
                # Calcular la altura para el vértice (coord_x, coord_z)
                y = altura(coord_x, coord_z)  # Calcular la altura usando la función determinista
                writer.writerow([coord_x, y, coord_z])
            writer.writerow([23, altura(2, z * separacion_z) + 10, z * separacion_z ])
# Ejecutar la generación de la malla
generar_malla()
