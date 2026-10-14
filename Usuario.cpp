#include "Usuario.h"

Usuario::Usuario() : id(0), indiceMatriz(-1) {}
Usuario::Usuario(int id, int indiceMatriz, const std::string& nombreUsuario,
                 const FechaHora& fechaNacimiento, const std::string& correo, const std::string& contrasena,
                 const std::string& fotoPerfil, const std::string& ciudad,
                 const std::string& pais, const FechaHora& fechaInscripcion)
    : id(id), indiceMatriz(indiceMatriz), nombreUsuario(nombreUsuario),
      fechaNacimiento(fechaNacimiento), correo(correo), contrasena(contrasena), fotoPerfil(fotoPerfil),
      ciudad(ciudad), pais(pais), fechaInscripcion(fechaInscripcion) {}

int Usuario::getId() const { return id; }
int Usuario::getIndiceMatriz() const { return indiceMatriz; }
const std::string& Usuario::getNombreUsuario() const { return nombreUsuario; }
const FechaHora& Usuario::getFechaNacimiento() const { return fechaNacimiento; }
const std::string& Usuario::getCorreo() const { return correo; }
const std::string& Usuario::getContrasena() const { return contrasena; }
const std::string& Usuario::getFotoPerfil() const { return fotoPerfil; }
const std::string& Usuario::getCiudad() const { return ciudad; }
const std::string& Usuario::getPais() const { return pais; }
const FechaHora& Usuario::getFechaInscripcion() const { return fechaInscripcion; }
int Usuario::getCantidadAlbumes() const { return albumes.obtenerTam(); }
const Album* Usuario::getAlbum(int posicion) const { return posicion >= 0 && posicion < albumes.obtenerTam() ? &albumes[posicion] : nullptr; }
Album* Usuario::getAlbum(int posicion) { return posicion >= 0 && posicion < albumes.obtenerTam() ? &albumes[posicion] : nullptr; }
Album* Usuario::buscarAlbumPorId(int idAlbum, long& iteraciones) { for (int i = 0; i < albumes.obtenerTam(); ++i) { ++iteraciones; if (albumes[i].getId() == idAlbum) return &albumes[i]; } return nullptr; }
bool Usuario::agregarAlbum(const Album& album) { if (album.getIdPropietario() != id) return false; albumes.agregar(album); return true; }
void Usuario::agregarMensajeEnviado(Mensaje* mensaje) { if (mensaje != nullptr) mensajesEnviados.agregar(mensaje); }
void Usuario::agregarMensajeRecibido(Mensaje* mensaje) { if (mensaje != nullptr) mensajesRecibidos.agregar(mensaje); }
int Usuario::getCantidadMensajesEnviados() const { return mensajesEnviados.obtenerTam(); }
int Usuario::getCantidadMensajesRecibidos() const { return mensajesRecibidos.obtenerTam(); }
long Usuario::memoriaUsada() const {
    long total = static_cast<long>(sizeof(*this)) + static_cast<long>(nombreUsuario.capacity() + correo.capacity() + contrasena.capacity() + fotoPerfil.capacity() + ciudad.capacity() + pais.capacity());
    for (int i = 0; i < albumes.obtenerTam(); ++i) total += albumes[i].memoriaUsada() - static_cast<long>(sizeof(Album));
    return total;
}
