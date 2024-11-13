import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.preprocessing import MinMaxScaler

# Verificar que se haya pasado un archivo como argumento
if len(sys.argv) != 2:
    print("Uso: python show.py archivo.csv")
    sys.exit(1)

# Cargar el archivo CSV desde los argumentos de línea de comandos
archivo_csv = sys.argv[1]
df = pd.read_csv(archivo_csv)

# Extraer las coordenadas X, Y, Z
X = df['x'].values
Y = df['y'].values  # Considerar Y como coordenadas horizontales
Z = df['z'].values  # Z será la altura

# Normalizar Y para que esté en el rango [0, 1] si se usa como color
scaler = MinMaxScaler()
Y_normalized = scaler.fit_transform(Y.reshape(-1, 1)).flatten()

# Crear el gráfico 3D
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

# Usar scatter para mostrar los puntos con el color normalizado
scatter = ax.scatter(X, Z, Y, c=Y_normalized, cmap='viridis', marker='o')

# Ajustar proporciones de los ejes X y Z
x_range = X.max() - X.min()
z_range = Z.max() - Z.min()
max_range = max(x_range, z_range)

# Configurar límites para que X y Z tengan la misma escala
x_middle = (X.max() + X.min()) / 2
z_middle = (Z.max() + Z.min()) / 2
ax.set_xlim(x_middle - max_range / 2, x_middle + max_range / 2)
ax.set_ylim(Z.min(), Z.max())  # Eje Y (altura) sin cambiar
ax.set_zlim(z_middle - max_range / 2, z_middle + max_range / 2)

# Añadir etiquetas
ax.set_xlabel('X Label')
ax.set_ylabel('Z Label')  # Cambiar etiqueta a Z
ax.set_zlabel('Y Label')   # Cambiar etiqueta a Y

# Mostrar la barra de color
fig.colorbar(scatter, label='Y Value')  # Etiqueta para la barra de color

plt.show()
