#include <iostream>
#include <cmath>
#include <fstream>

// Función gaussiana para generar irregularidad en el terreno
float gaussiana(float x, float z) {
    // Centro de la distribución
    const float mu = 4.2f;
    const float amplitud_gaussiana = 1.0f;
    const float sigma = 2.0f;
    // Función gaussiana para modificar la altura
    return amplitud_gaussiana * std::exp(-((x - mu) * (x - mu) + (z - mu) * (z - mu)) / (2 * sigma * sigma));
}

// Función para generar la altura utilizando una combinación de funciones sinusoidales y gaussiana
float altura(float x, float z) {
    // Parámetros
    const float frecuencia_seno_x = 0.15f;  // Frecuencia de las ondas en la dirección X
    const float frecuencia_seno_z = 0.04f;  // Frecuencia de las ondas en la dirección Z
    const float amplitud_seno_x = 2.0f;    // Amplitud de las ondas en la dirección X
    const float amplitud_seno_z = 1.5f;    // Amplitud de las ondas en la dirección Z
    const float pendiente = -0.2f;

    // Generamos una onda sinusoidal para la dirección X
    float onda_seno_x = amplitud_seno_x * std::sin(frecuencia_seno_x * (x + 20));

    // Generamos una onda sinusoidal para la dirección Z
    float onda_seno_z = amplitud_seno_z * std::cos(frecuencia_seno_z * z);

    // Agregamos la irregularidad de la función gaussiana
    float irregularidad = gaussiana(x, z);

    // Aplicamos la pendiente, la cual varía la altura con base en la coordenada Z
    float pendiente_effect = pendiente * z;

    // La altura final es la combinación de las ondas, la irregularidad gaussiana y la pendiente
    return onda_seno_x + onda_seno_z + irregularidad + pendiente_effect;
}