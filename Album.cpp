#include "Album.h"

Album::Album() : id(0), idPropietario(0), privacidad(Privacidad::Privado) {}
Album::Album(int id, int idPropietario, const std::string& nombre,
             const std::string& rutaAlmacenamiento, Privacidad privacidad)
    : id(id), idPropietario(idPropietario), nombre(nombre), rutaAlmacenamiento(rutaAlmacenamiento), privacidad(privacidad) {}
int Album::getId() const { return id; }
int Album::getIdPropietario() const { return idPropietario; }
const std::string& Album::getNombre() const { return nombre; }
const std::string& Album::getRutaAlmacenamiento() const { return rutaAlmacenamiento; }
Privacidad Album::getPrivacidad() const { return privacidad; }
int Album::getCantidadFotos() const { return fotos.obtenerTam(); }
const Foto* Album::getFoto(int posicion) const { return posicion >= 0 && posicion < fotos.obtenerTam() ? &fotos[posicion] : nullptr; }
Foto* Album::buscarFotoPorId(int idFoto, long& iteraciones) { for (int i = 0; i < fotos.obtenerTam(); ++i) { ++iteraciones; if (fotos[i].getId() == idFoto) return &fotos[i]; } return nullptr; }
bool Album::agregarFoto(const Foto& foto) { if (foto.getId() <= 0) return false; fotos.agregar(foto); return true; }
bool Album::esVisiblePara(bool esPropietario, bool esAmigo) const { return privacidad == Privacidad::Publico || esPropietario || (privacidad == Privacidad::Amigos && esAmigo); }
long Album::memoriaUsada() const { long total = static_cast<long>(sizeof(*this)) + static_cast<long>(nombre.capacity() + rutaAlmacenamiento.capacity()) + static_cast<long>(fotos.obtenerCapacidad()) * static_cast<long>(sizeof(Foto)); for (int i = 0; i < fotos.obtenerTam(); ++i) total += fotos[i].memoriaUsada() - static_cast<long>(sizeof(Foto)); return total; }
