#ifndef ARREGLODINAMICO_H
#define ARREGLODINAMICO_H

template <typename T>
class ArregloDinamico {
private:
    T* datos;
    int tam;
    int capacidad;
    mutable long iteraciones; // para MedidorRecursos

    void ampliar() {
        int nuevaCap = (capacidad == 0) ? 4 : capacidad * 2;
        T* nuevo = new T[nuevaCap];
        for (int i = 0; i < tam; i++) {
            nuevo[i] = datos[i];
            iteraciones++;
        }
        delete[] datos;
        datos = nuevo;
        capacidad = nuevaCap;
    }

public:
    ArregloDinamico() : datos(nullptr), tam(0), capacidad(0), iteraciones(0) {}

    explicit ArregloDinamico(int capInicial)
        : datos(nullptr), tam(0), capacidad(0), iteraciones(0) {
        if (capInicial > 0) {
            datos = new T[capInicial];
            capacidad = capInicial;
        }
    }

    // Constructor de copia (copia profunda del almacenamiento)
    ArregloDinamico(const ArregloDinamico& otro)
        : datos(nullptr), tam(otro.tam), capacidad(otro.capacidad), iteraciones(0) {
        if (capacidad > 0) {
            datos = new T[capacidad];
            for (int i = 0; i < tam; i++) {
                datos[i] = otro.datos[i];
                iteraciones++;
            }
        }
    }

    ~ArregloDinamico() { delete[] datos; }

    ArregloDinamico& operator=(const ArregloDinamico& otro) {
        if (this != &otro) {
            T* nuevo = (otro.capacidad > 0) ? new T[otro.capacidad] : nullptr;
            for (int i = 0; i < otro.tam; i++) {
                nuevo[i] = otro.datos[i];
                iteraciones++;
            }
            delete[] datos;
            datos = nuevo;
            tam = otro.tam;
            capacidad = otro.capacidad;
        }
        return *this;
    }

    void agregar(const T& valor) {
        if (tam == capacidad) ampliar();
        datos[tam++] = valor;
    }

    // Conserva la capacidad reservada para poder reutilizar el arreglo al recargar datos.
    void limpiar() { tam = 0; }

    bool eliminarEn(int indice) {
        if (indice < 0 || indice >= tam) return false;
        for (int i = indice; i < tam - 1; ++i) datos[i] = datos[i + 1];
        --tam;
        return true;
    }

    T& operator[](int i) { return datos[i]; }
    const T& operator[](int i) const { return datos[i]; }

    int obtenerTam() const { return tam; }
    int obtenerCapacidad() const { return capacidad; }
    bool estaVacio() const { return tam == 0; }

    long obtenerIteraciones() const { return iteraciones; }
    void reiniciarIteraciones() { iteraciones = 0; }

    // Memoria que ocupa este arreglo (objeto + almacenamiento dinámico)
    long memoriaUsada() const {
        return (long)sizeof(*this) + (long)capacidad * (long)sizeof(T);
    }
};

#endif
