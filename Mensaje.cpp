#include "Mensaje.h"
Mensaje::Mensaje() : emisor(nullptr), receptor(nullptr), leido(false) {}
Mensaje::Mensaje(Usuario* emisor, Usuario* receptor, const std::string& texto, const FechaHora& envio, const FechaHora& recepcion) : emisor(emisor), receptor(receptor), texto(texto), fechaEnvio(envio), fechaRecepcion(recepcion), leido(false) {}
const Usuario* Mensaje::obtenerEmisor() const { return emisor; }
const Usuario* Mensaje::obtenerReceptor() const { return receptor; }
const std::string& Mensaje::getTexto() const { return texto; }
const FechaHora& Mensaje::getFechaEnvio() const { return fechaEnvio; }
const FechaHora& Mensaje::getFechaRecepcion() const { return fechaRecepcion; }
bool Mensaje::estaLeido() const { return leido; }
void Mensaje::marcarLeido() { leido = true; }
