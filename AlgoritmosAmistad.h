#ifndef ALGORITMOSAMISTAD_H
#define ALGORITMOSAMISTAD_H

#include "MatrizAmistades.h"

// Funciones sin dependencias de Usuario. RedSocial traduce sus indices a usuarios.
class AlgoritmosAmistad {
private:
    static unsigned int siguienteAleatorio(unsigned int& semilla, long& iteraciones);

public:
    // Escribe indices de candidatos y sus cantidades de amigos comunes, en orden
    // descendente. No incluye al usuario ni a sus amigos actuales.
    static int sugerir(const MatrizAmistades& matriz, int usuario, int maximo,
                       int* indicesSalida, int* comunesSalida, long& iteraciones);

    // Escoge hasta tres posiciones diferentes de candidatos usando peso 1, 2.5 y
    // 3.5 para 0-10, 11-20 y >=21 amigos comunes, respectivamente.
    // semilla se conserva entre renovaciones para no generar un patron fijo.
    static int seleccionarDestacados(const int* candidatos, const int* comunes,
                                     int cantidadCandidatos, int* salida,
                                     int capacidadSalida, unsigned int& semilla,
                                     long& iteraciones);
};

#endif
