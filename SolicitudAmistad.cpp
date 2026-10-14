#include "SolicitudAmistad.h"
#include "Usuario.h"

SolicitudAmistad::SolicitudAmistad() : remitente(nullptr), destinatario(nullptr), estado(EstadoSolicitud::Pendiente) {}
SolicitudAmistad::SolicitudAmistad(Usuario* remitente, Usuario* destinatario, const FechaHora& fechaHora)
    : remitente(remitente), destinatario(destinatario), fechaHora(fechaHora), estado(EstadoSolicitud::Pendiente) {}
const Usuario* SolicitudAmistad::obtenerSolicitante() const { return remitente; }
const Usuario* SolicitudAmistad::obtenerDestinatario() const { return destinatario; }
int SolicitudAmistad::getIdRemitente() const { return remitente == nullptr ? 0 : remitente->getId(); }
int SolicitudAmistad::getIdDestinatario() const { return destinatario == nullptr ? 0 : destinatario->getId(); }
const FechaHora& SolicitudAmistad::getFechaHora() const { return fechaHora; }
EstadoSolicitud SolicitudAmistad::getEstado() const { return estado; }
bool SolicitudAmistad::estaPendiente() const { return estado == EstadoSolicitud::Pendiente; }
void SolicitudAmistad::aceptar() { estado = EstadoSolicitud::Aceptada; }
void SolicitudAmistad::rechazar() { estado = EstadoSolicitud::Rechazada; }
void SolicitudAmistad::establecerEstado(EstadoSolicitud nuevoEstado) { estado = nuevoEstado; }
