#include "Persistencia.h"

Persistencia::Persistencia() {}

void Persistencia::configurar(const std::string& ruta) {
    if (salida.is_open()) salida.close();
    rutaLog = ruta;
}

const std::string& Persistencia::getRuta() const { return rutaLog; }

bool Persistencia::agregarLinea(const std::string& linea) {
    if (rutaLog.empty()) return false;
    if (!salida.is_open()) salida.open(rutaLog.c_str(), std::ios::app);
    if (!salida.is_open()) return false;
    salida << linea << ",FIN\n";
    salida.flush();
    return salida.good();
}

bool Persistencia::abrirLectura() {
    if (rutaLog.empty()) return false;
    entrada.clear();
    entrada.open(rutaLog.c_str());
    return entrada.is_open();
}

bool Persistencia::leerLinea(std::string& linea) {
    return static_cast<bool>(std::getline(entrada, linea));
}

void Persistencia::cerrarLectura() {
    if (entrada.is_open()) entrada.close();
}