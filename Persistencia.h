#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include <string>
#include <fstream>

// Gestiona el registro de cambios (cambios.log). Solo se agrega al final durante
// la ejecución y solo se lee una vez, al iniciar. Nunca se usa para consultar datos.
class Persistencia {
private:
    std::string rutaLog;
    std::ofstream salida;
    std::ifstream entrada;

public:
    Persistencia();

    void configurar(const std::string& ruta);
    const std::string& getRuta() const;

    // Agrega "linea,FIN" al final del registro. true si la escritura fue comprobada.
    bool agregarLinea(const std::string& linea);

    // Lectura secuencial para la recuperación.
    bool abrirLectura();                    // false si el registro aún no existe
    bool leerLinea(std::string& linea);     // false al terminar
    void cerrarLectura();
};

#endif