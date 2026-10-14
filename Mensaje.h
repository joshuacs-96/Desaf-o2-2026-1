#ifndef MENSAJE_H
#define MENSAJE_H

#include <string>
#include "FechaHora.h"
class Usuario;

class Mensaje {
private:
    Usuario* emisor;
    Usuario* receptor;
    std::string texto;
    FechaHora fechaEnvio;
    FechaHora fechaRecepcion;
    bool leido;
public:
    Mensaje();
    Mensaje(Usuario* emisor, Usuario* receptor, const std::string& texto, const FechaHora& envio, const FechaHora& recepcion);
    const Usuario* obtenerEmisor() const;
    const Usuario* obtenerReceptor() const;
    const std::string& getTexto() const;
    const FechaHora& getFechaEnvio() const;
    const FechaHora& getFechaRecepcion() const;
    bool estaLeido() const;
    void marcarLeido();
};

#endif
