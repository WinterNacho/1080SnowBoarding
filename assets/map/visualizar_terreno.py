import csv
import sys
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.spatial import Delaunay

def visualizar_terreno(archivo_csv):
    # Listas para almacenar coordenadas x, y, z
    x_coords = []
    y_coords = []
    z_coords = []

    # Leer los datos del archivo CSV
    with open(archivo_csv, mode='r') as file:
        reader = csv.reader(file)
        next(reader)  # Saltar la cabecera
        for row in reader:
            x, y, z = map(float, row)
            x_coords.append(x)
            y_coords.append(y)
            z_coords.append(z)

    # Convertir listas a arreglos numpy para Delaunay
    points2D = np.array(list(zip(x_coords, z_coords)))
    y_coords = np.array(y_coords)

    # Triangulación de Delaunay
    tri = Delaunay(points2D)

    # Configurar la figura 3D
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    # Graficar la superficie triangular
    ax.plot_trisurf(x_coords, z_coords, y_coords, triangles=tri.simplices, cmap='terrain', edgecolor='grey', linewidth=0.2)

    # Configurar etiquetas y título
    ax.set_xlabel('X')
    ax.set_ylabel('Z')
    ax.set_zlabel('Y')
    ax.set_title('Visualización del Terreno con Triángulos')

    # Configurar límites y relación de aspecto 1:1 para x, y y z
    max_range = max(max(x_coords) - min(x_coords), max(z_coords) - min(z_coords), max(y_coords) - min(y_coords))
    mid_x = (max(x_coords) + min(x_coords)) / 2
    mid_z = (max(z_coords) + min(z_coords)) / 2
    mid_y = (max(y_coords) + min(y_coords)) / 2

    # Ajustar límites para que la relación entre los ejes x, y, z sea igual
    ax.set_xlim(mid_x - max_range / 2, mid_x + max_range / 2)
    ax.set_ylim(mid_z - max_range / 2, mid_z + max_range / 2)
    ax.set_zlim(mid_y - max_range / 2, mid_y + max_range / 2)

    # Mostrar la gráfica
    plt.show()

# Ejecutar solo si se llama desde la línea de comandos
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python visualizar_terreno.py archivo.csv")
    else:
        archivo_csv = sys.argv[1]
        visualizar_terreno(archivo_csv)
