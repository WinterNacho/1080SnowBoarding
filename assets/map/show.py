import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from scipy.interpolate import griddata

# Verificar que se haya pasado un archivo como argumento
if len(sys.argv) != 2:
    print("Uso: python show.py archivo.csv")
    sys.exit(1)

# Cargar el archivo CSV desde los argumentos de línea de comandos
archivo_csv = sys.argv[1]
df = pd.read_csv(archivo_csv)

# Crear una malla de puntos a partir de los datos
X = df['x'].values
Y = df['y'].values
Z = df['z'].values

# Crear una malla de coordenadas
X_unique = np.unique(X)
Y_unique = np.unique(Y)
X_mesh, Y_mesh = np.meshgrid(X_unique, Y_unique)

# Interpolar Z para la malla de coordenadas
Z_mesh = griddata((X, Y), Z, (X_mesh, Y_mesh), method='linear')

# Crear el gráfico 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
surf = ax.plot_surface(X_mesh, Y_mesh, Z_mesh, cmap='viridis')

# Añadir etiquetas
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

# Mostrar la barra de color
fig.colorbar(surf)

plt.show()
