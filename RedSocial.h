#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include "ArregloDinamico.h"
#include "Usuario.h"
#include "SolicitudAmistad.h"
#include "Mensaje.h"
#include "MatrizAmistades.h"
#include "MedidorRecursos.h"

// Posee los datos del dominio. Los CSV relacionan entidades por ID; la matriz usa
// exclusivamente el indiceMatriz asignado durante la carga de usuarios.
class RedSocial {
private:
    ArregloDinamico<Usuario> usuarios;
    ArregloDinamico<SolicitudAmistad> solicitudes;
    ArregloDinamico<Mensaje*> mensajes;
    MatrizAmistades amistades;
    mutable MedidorRecursos medidor;
    unsigned int semilla;

    int posicionUsuarioPorId(int id, long& iteraciones) const;
    int posicionAlbumPorId(int id, long& iteraciones) const;
    Album* buscarAlbumPorId(int id, long& iteraciones);
    bool haySolicitudPendienteEntre(int idA, int idB, long& iteraciones) const;
    bool cargarUsuarios(const std::string& ruta);
    bool cargarAmistades(const std::string& ruta);
    bool cargarSolicitudes(const std::string& ruta);
    bool cargarAlbumes(const std::string& ruta);
    bool cargarFotos(const std::string& ruta);
    bool cargarLikes(const std::string& ruta);
    bool cargarMensajes(const std::string& ruta);

public:
    RedSocial();
    ~RedSocial();
    bool cargarDesdeDirectorio(const std::string& directorio);

    const Usuario* buscarUsuarioPorId(int id);
    const Usuario* buscarUsuarioPorNombre(const std::string& nombre);
    int getCantidadUsuarios() const;
    const MedidorRecursos& getMedidor() const;
    long memoriaUsada() const;

    bool sonAmigos(int idUsuarioA, int idUsuarioB);
    bool enviarSolicitud(int idRemitente, int idDestinatario, const FechaHora& fechaHora);
    bool aceptarSolicitud(int idDestinatario, int idRemitente);
    const Usuario* autenticar(const std::string& nombre, const std::string& contrasena);
    bool rechazarSolicitud(int idDestinatario, int idRemitente);
    int obtenerSolicitudesPendientes(int idDestinatario, int* idsRemitentes, int capacidad);
    int obtenerSugerencias(int idUsuario, int maximo, int* idsSalida, int* comunesSalida);
    int obtenerMejoresAmigos(int idUsuario, int* idsSalida, int capacidad);
    int obtenerAlbumesVisibles(int idVisitante, int idPropietario, int* idsAlbumes, int capacidad);
    bool alternarLike(int idUsuario, int idFoto);
    int enviarMensaje(int idRemitente, int idDestinatario, const std::string& texto, const FechaHora& fecha);
    int leerNuevosMensajes(int idUsuario, Mensaje** salida, int capacidad);
    int buscarMensajes(int idUsuario, const FechaHora& desde, const FechaHora& hasta, const Mensaje** salida, int capacidad);
};

#endif
