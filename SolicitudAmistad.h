#ifndef SOLICITUDAMISTAD_H
#define SOLICITUDAMISTAD_H

#include "FechaHora.h"
class Usuario;

enum class EstadoSolicitud { Pendiente, Aceptada, Rechazada };

class SolicitudAmistad {
private:
    Usuario* remitente;
    Usuario* destinatario;
    FechaHora fechaHora;
    EstadoSolicitud estado;

public:
    SolicitudAmistad();
    SolicitudAmistad(Usuario* remitente, Usuario* destinatario, const FechaHora& fechaHora);
    const Usuario* obtenerSolicitante() const;
    const Usuario* obtenerDestinatario() const;
    int getIdRemitente() const;
    int getIdDestinatario() const;
    const FechaHora& getFechaHora() const;
    EstadoSolicitud getEstado() const;
    bool estaPendiente() const;
    void aceptar();
    void rechazar();
    void establecerEstado(EstadoSolicitud nuevoEstado);
};

#endif
