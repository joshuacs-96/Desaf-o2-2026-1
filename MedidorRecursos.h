#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

class MedidorRecursos {
private:
    long iteraciones;
    long llamadasExternas;
    long bytesActuales;

public:
    MedidorRecursos();
    void reiniciarOperacion();
    void sumarIteraciones(long cantidad = 1);
    void registrarLlamadaExterna(long cantidad = 1);
    // La memoria no se acumula: representa el total vivo al consultar la medicion.
    void actualizarMemoriaActual(long bytes);
    long getIteraciones() const;
    long getLlamadasExternas() const;
    long getBytesActuales() const;
};

#endif
