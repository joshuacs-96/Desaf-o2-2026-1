#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "FechaHora.h"
#include "ArregloDinamico.h"
#include "Album.h"
class Mensaje;

class Usuario {
private:
    int id;
    int indiceMatriz;
    std::string nombreUsuario;
    FechaHora fechaNacimiento;
    std::string correo;
    std::string contrasena;
    std::string fotoPerfil;
    std::string ciudad;
    std::string pais;
    FechaHora fechaInscripcion;
    ArregloDinamico<Album> albumes;
    ArregloDinamico<Mensaje*> mensajesEnviados;
    ArregloDinamico<Mensaje*> mensajesRecibidos;

public:
    Usuario();
    Usuario(int id, int indiceMatriz, const std::string& nombreUsuario,
            const FechaHora& fechaNacimiento, const std::string& correo, const std::string& contrasena,
            const std::string& fotoPerfil, const std::string& ciudad,
            const std::string& pais, const FechaHora& fechaInscripcion);

    int getId() const;
    int getIndiceMatriz() const;
    const std::string& getNombreUsuario() const;
    const FechaHora& getFechaNacimiento() const;
    const std::string& getCorreo() const;
    const std::string& getContrasena() const;
    const std::string& getFotoPerfil() const;
    const std::string& getCiudad() const;
    const std::string& getPais() const;
    const FechaHora& getFechaInscripcion() const;
    int getCantidadAlbumes() const;
    const Album* getAlbum(int posicion) const;
    Album* getAlbum(int posicion);
    Album* buscarAlbumPorId(int idAlbum, long& iteraciones);
    bool agregarAlbum(const Album& album);
    void agregarMensajeEnviado(Mensaje* mensaje);
    void agregarMensajeRecibido(Mensaje* mensaje);
    int getCantidadMensajesEnviados() const;
    int getCantidadMensajesRecibidos() const;
    long memoriaUsada() const;
    bool verificarContrasena(const std::string& intento) const;
    Mensaje* getMensajeRecibido(int posicion) const;
};

#endif
