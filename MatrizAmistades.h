#ifndef MATRIZAMISTADES_H
#define MATRIZAMISTADES_H

#include <cstdint>

// Matriz de adyacencia no dirigida empaquetada en bloques de 64 bits.
// La posicion i,j indica si los usuarios con indices i y j son amigos.
class MatrizAmistades {
private:
    std::uint64_t* bloques;
    int cantidadUsuarios;
    int bloquesPorFila;

    bool indiceValido(int indice) const;
    long indiceBloque(int fila, int columna) const;
    std::uint64_t mascara(int columna) const;
    void liberar();
    void copiarDesde(const MatrizAmistades& otra);

public:
    MatrizAmistades();
    explicit MatrizAmistades(int usuarios);
    MatrizAmistades(const MatrizAmistades& otra);
    MatrizAmistades& operator=(const MatrizAmistades& otra);
    ~MatrizAmistades();

    bool inicializar(int usuarios);
    int obtenerCantidadUsuarios() const;
    int obtenerBloquesPorFila() const;
    long memoriaUsada() const;

    bool sonAmigos(int usuarioA, int usuarioB, long& iteraciones) const;
    bool establecerAmistad(int usuarioA, int usuarioB, long& iteraciones);
    bool eliminarAmistad(int usuarioA, int usuarioB, long& iteraciones);

    // Cuenta los amigos compartidos comparando las dos filas bloque por bloque.
    // Devuelve -1 cuando algun indice es invalido.
    int contarAmigosComunes(int usuarioA, int usuarioB, long& iteraciones) const;

    // Copia en salida los indices de los amigos de usuario. Retorna cuantos copio.
    int obtenerAmigos(int usuario, int* salida, int capacidadSalida,
                      long& iteraciones) const;
};

#endif
