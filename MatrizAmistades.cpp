#include "MatrizAmistades.h"

MatrizAmistades::MatrizAmistades()
    : bloques(nullptr), cantidadUsuarios(0), bloquesPorFila(0) {}

MatrizAmistades::MatrizAmistades(int usuarios)
    : bloques(nullptr), cantidadUsuarios(0), bloquesPorFila(0) {
    inicializar(usuarios);
}

MatrizAmistades::MatrizAmistades(const MatrizAmistades& otra)
    : bloques(nullptr), cantidadUsuarios(0), bloquesPorFila(0) {
    copiarDesde(otra);
}

MatrizAmistades& MatrizAmistades::operator=(const MatrizAmistades& otra) {
    if (this != &otra) {
        liberar();
        copiarDesde(otra);
    }
    return *this;
}

MatrizAmistades::~MatrizAmistades() {
    liberar();
}

void MatrizAmistades::liberar() {
    delete[] bloques;
    bloques = nullptr;
    cantidadUsuarios = 0;
    bloquesPorFila = 0;
}

void MatrizAmistades::copiarDesde(const MatrizAmistades& otra) {
    cantidadUsuarios = otra.cantidadUsuarios;
    bloquesPorFila = otra.bloquesPorFila;
    long total = static_cast<long>(cantidadUsuarios) * bloquesPorFila;
    bloques = total > 0 ? new std::uint64_t[total] : nullptr;
    for (long i = 0; i < total; ++i) {
        bloques[i] = otra.bloques[i];
    }
}

bool MatrizAmistades::inicializar(int usuarios) {
    if (usuarios < 0) return false;
    liberar();
    cantidadUsuarios = usuarios;
    bloquesPorFila = (usuarios + 63) / 64;
    long total = static_cast<long>(cantidadUsuarios) * bloquesPorFila;
    bloques = total > 0 ? new std::uint64_t[total] : nullptr;
    for (long i = 0; i < total; ++i) {
        bloques[i] = 0;
    }
    return true;
}

int MatrizAmistades::obtenerCantidadUsuarios() const { return cantidadUsuarios; }
int MatrizAmistades::obtenerBloquesPorFila() const { return bloquesPorFila; }

long MatrizAmistades::memoriaUsada() const {
    return static_cast<long>(sizeof(*this)) +
           static_cast<long>(cantidadUsuarios) * bloquesPorFila * sizeof(std::uint64_t);
}

bool MatrizAmistades::indiceValido(int indice) const {
    return indice >= 0 && indice < cantidadUsuarios;
}

long MatrizAmistades::indiceBloque(int fila, int columna) const {
    return static_cast<long>(fila) * bloquesPorFila + (columna / 64);
}

std::uint64_t MatrizAmistades::mascara(int columna) const {
    return std::uint64_t(1) << (columna % 64);
}

bool MatrizAmistades::sonAmigos(int usuarioA, int usuarioB, long& iteraciones) const {
    ++iteraciones;
    if (!indiceValido(usuarioA) || !indiceValido(usuarioB) || usuarioA == usuarioB) {
        return false;
    }
    return (bloques[indiceBloque(usuarioA, usuarioB)] & mascara(usuarioB)) != 0;
}

bool MatrizAmistades::establecerAmistad(int usuarioA, int usuarioB, long& iteraciones) {
    ++iteraciones;
    if (!indiceValido(usuarioA) || !indiceValido(usuarioB) || usuarioA == usuarioB) {
        return false;
    }
    bloques[indiceBloque(usuarioA, usuarioB)] |= mascara(usuarioB);
    bloques[indiceBloque(usuarioB, usuarioA)] |= mascara(usuarioA);
    return true;
}

bool MatrizAmistades::eliminarAmistad(int usuarioA, int usuarioB, long& iteraciones) {
    ++iteraciones;
    if (!indiceValido(usuarioA) || !indiceValido(usuarioB) || usuarioA == usuarioB) {
        return false;
    }
    bloques[indiceBloque(usuarioA, usuarioB)] &= ~mascara(usuarioB);
    bloques[indiceBloque(usuarioB, usuarioA)] &= ~mascara(usuarioA);
    return true;
}

int MatrizAmistades::contarAmigosComunes(int usuarioA, int usuarioB,
                                          long& iteraciones) const {
    if (!indiceValido(usuarioA) || !indiceValido(usuarioB)) return -1;
    int comunes = 0;
    for (int bloque = 0; bloque < bloquesPorFila; ++bloque) {
        ++iteraciones;
        std::uint64_t coincidencias =
            bloques[static_cast<long>(usuarioA) * bloquesPorFila + bloque] &
            bloques[static_cast<long>(usuarioB) * bloquesPorFila + bloque];
        while (coincidencias != 0) {
            ++iteraciones;
            coincidencias &= (coincidencias - 1);
            ++comunes;
        }
    }
    return comunes;
}

int MatrizAmistades::obtenerAmigos(int usuario, int* salida, int capacidadSalida,
                                   long& iteraciones) const {
    if (!indiceValido(usuario) || salida == nullptr || capacidadSalida < 0) return 0;
    int encontrados = 0;
    for (int candidato = 0; candidato < cantidadUsuarios; ++candidato) {
        ++iteraciones;
        if (sonAmigos(usuario, candidato, iteraciones)) {
            if (encontrados < capacidadSalida) salida[encontrados] = candidato;
            ++encontrados;
        }
    }
    return encontrados;
}
