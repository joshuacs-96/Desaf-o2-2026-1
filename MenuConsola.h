#ifndef MENUCONSOLA_H
#define MENUCONSOLA_H

#include <string>
#include "RedSocial.h"

// Interfaz de consola. No contiene reglas del negocio: pide datos, llama a
// RedSocial y muestra resultados y métricas. No es dueña de la red.
class MenuConsola {
private:
    RedSocial& red;
    const Usuario* sesion;       // usuario autenticado (nullptr si no hay sesión)
    long itCap;                  // métricas capturadas tras cada operación
    long memCap;
    long extCap;

    std::string leerTexto(const std::string& mensaje) const;
    int leerEntero(const std::string& mensaje) const;
    bool leerFecha(const std::string& mensaje, const std::string& horaFija, FechaHora& salida) const;

    void capturarMetricas();
    void mostrarMetricas() const;

    void iniciarSesion();
    void menuPrincipal();
    void mostrarAmigosDestacados();
    void administrarAmistades();
    void enviarSolicitudPorNombre();
    void gestionarSolicitudesRecibidas();
    void verSugerencias();
    void visitarPerfil();
    void administrarMensajes();
    void enviarMensaje();
    void leerNuevos();
    void buscarPorFechas();

public:
    explicit MenuConsola(RedSocial& red);
    void ejecutar();
};

#endif