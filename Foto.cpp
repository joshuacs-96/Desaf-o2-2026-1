#include "Foto.h"

Foto::Foto() : id(0), tamanoBytes(0) {}
Foto::Foto(int id, const FechaHora& fechaCaptura, long tamanoBytes, const std::string& ruta)
    : id(id), fechaCaptura(fechaCaptura), tamanoBytes(tamanoBytes), ruta(ruta) {}
int Foto::getId() const { return id; }
const FechaHora& Foto::getFechaCaptura() const { return fechaCaptura; }
long Foto::getTamanoBytes() const { return tamanoBytes; }
const std::string& Foto::getRuta() const { return ruta; }
int Foto::getCantidadLikes() const { return idsLikes.obtenerTam(); }
bool Foto::tieneLikeDe(int idUsuario, long& iteraciones) const { for (int i = 0; i < idsLikes.obtenerTam(); ++i) { ++iteraciones; if (idsLikes[i] == idUsuario) return true; } return false; }
bool Foto::agregarLikeCargado(int idUsuario, long& iteraciones) { if (idUsuario <= 0 || tieneLikeDe(idUsuario, iteraciones)) return false; idsLikes.agregar(idUsuario); return true; }
bool Foto::alternarLike(int idUsuario, long& iteraciones) {
    if (idUsuario <= 0) return false;
    for (int i = 0; i < idsLikes.obtenerTam(); ++i) {
        ++iteraciones;
        if (idsLikes[i] == idUsuario) { idsLikes.eliminarEn(i); return false; }
    }
    idsLikes.agregar(idUsuario); return true;
}
long Foto::memoriaUsada() const { return static_cast<long>(sizeof(*this)) + static_cast<long>(ruta.capacity()) + static_cast<long>(idsLikes.obtenerCapacidad()) * static_cast<long>(sizeof(int)); }
