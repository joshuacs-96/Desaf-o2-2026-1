#include "AlgoritmosAmistad.h"

unsigned int AlgoritmosAmistad::siguienteAleatorio(unsigned int& semilla,
                                                    long& iteraciones) {
    ++iteraciones;
    semilla = semilla * 1664525u + 1013904223u;
    return semilla;
}

int AlgoritmosAmistad::sugerir(const MatrizAmistades& matriz, int usuario,
                               int maximo, int* indicesSalida, int* comunesSalida,
                               long& iteraciones) {
    if (maximo <= 0 || indicesSalida == nullptr || comunesSalida == nullptr ||
        usuario < 0 || usuario >= matriz.obtenerCantidadUsuarios()) return 0;

    int encontrados = 0;
    for (int candidato = 0; candidato < matriz.obtenerCantidadUsuarios(); ++candidato) {
        ++iteraciones;
        if (candidato == usuario || matriz.sonAmigos(usuario, candidato, iteraciones)) {
            continue;
        }
        int comunes = matriz.contarAmigosComunes(usuario, candidato, iteraciones);
        if (comunes <= 0) continue;

        // Insercion ordenada: conserva solo los maximo mejores candidatos.
        int posicion = encontrados;
        if (posicion >= maximo) posicion = maximo - 1;
        while (posicion > 0 && comunes > comunesSalida[posicion - 1]) {
            ++iteraciones;
            if (posicion < maximo) {
                indicesSalida[posicion] = indicesSalida[posicion - 1];
                comunesSalida[posicion] = comunesSalida[posicion - 1];
            }
            --posicion;
        }
        if (encontrados < maximo || posicion < maximo - 1) {
            indicesSalida[posicion] = candidato;
            comunesSalida[posicion] = comunes;
            if (encontrados < maximo) ++encontrados;
        }
    }
    return encontrados;
}

int AlgoritmosAmistad::seleccionarDestacados(const int* candidatos,
                                             const int* comunes,
                                             int cantidadCandidatos, int* salida,
                                             int capacidadSalida,
                                             unsigned int& semilla,
                                             long& iteraciones) {
    if (candidatos == nullptr || comunes == nullptr || salida == nullptr ||
        cantidadCandidatos <= 0 || capacidadSalida <= 0) return 0;

    int limite = cantidadCandidatos < capacidadSalida ? cantidadCandidatos : capacidadSalida;
    if (limite > 3) limite = 3;
    bool* usado = new bool[cantidadCandidatos];
    for (int i = 0; i < cantidadCandidatos; ++i) {
        ++iteraciones;
        usado[i] = false;
    }

    int seleccionados = 0;
    while (seleccionados < limite) {
        int pesoTotal = 0;
        for (int i = 0; i < cantidadCandidatos; ++i) {
            ++iteraciones;
            if (!usado[i]) {
                pesoTotal += comunes[i] >= 21 ? 35 : (comunes[i] >= 11 ? 25 : 10);
            }
        }
        if (pesoTotal == 0) break;
        int sorteo = static_cast<int>(siguienteAleatorio(semilla, iteraciones) % pesoTotal);
        int acumulado = 0;
        for (int i = 0; i < cantidadCandidatos; ++i) {
            ++iteraciones;
            if (usado[i]) continue;
            acumulado += comunes[i] >= 21 ? 35 : (comunes[i] >= 11 ? 25 : 10);
            if (sorteo < acumulado) {
                usado[i] = true;
                salida[seleccionados++] = candidatos[i];
                break;
            }
        }
    }
    delete[] usado;
    return seleccionados;
}
