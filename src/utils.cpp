#include <iostream>
#include <cmath>
#include <fstream>

// Función gaussiana para generar irregularidad en el terreno
float gaussiana(float x, float z) {
    const float mu = 4.2f;
    const float amplitud_gaussiana = 1.0f;
    const float sigma = 2.0f;
    return amplitud_gaussiana * std::exp(-((x - mu) * (x - mu) + (z - mu) * (z - mu)) / (2 * sigma * sigma));
}

// Función para generar la altura utilizando una combinación de funciones sinusoidales y gaussiana
float altura(float x, float z) {
    const float frecuencia_seno_x = 0.15f;  
    const float frecuencia_seno_z = 0.04f;
    const float amplitud_seno_x = 2.0f;
    const float amplitud_seno_z = 1.5f;
    const float pendiente = -0.2f;


    float onda_seno_x = amplitud_seno_x * std::sin(frecuencia_seno_x * (x + 20));
    float onda_seno_z = amplitud_seno_z * std::cos(frecuencia_seno_z * z);


    float irregularidad = gaussiana(x, z);

    float pendiente_effect = pendiente * z;

    return onda_seno_x + onda_seno_z + irregularidad + pendiente_effect + 0.6; // 
}

void toZero(float& valorActual, float paso) {
    if (valorActual > 0) {
        valorActual = std::max(0.0f, valorActual - paso);
    }
    else if (valorActual < 0) {
        valorActual = std::min(0.0f, valorActual + paso);
    }
}