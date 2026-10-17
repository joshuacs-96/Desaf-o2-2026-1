#include "MenuConsola.h"
#include "UtilCsv.h"

#include <iostream>

MenuConsola::MenuConsola(RedSocial& r) : red(r), sesion(nullptr), itCap(0), memCap(0), extCap(0) {}

// ---------- utilidades de entrada ----------
std::string MenuConsola::leerTexto(const std::string& mensaje) const {
    std::cout << mensaje;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

int MenuConsola::leerEntero(const std::string& mensaje) const {
    return UtilCsv::aEntero(leerTexto(mensaje));
}

// horaFija: "" para pedir fecha y hora completas; "00:00:00" o "23:59:59" para pedir solo el día.
bool MenuConsola::leerFecha(const std::string& mensaje, const std::string& horaFija, FechaHora& salida) const {
    std::string t = leerTexto(mensaje);
    if (!horaFija.empty()) t += " " + horaFija;
    if (!FechaHora::desdeTexto(t, salida)) {
        std::cout << "Fecha invalida. Use el formato indicado.\n";
        return false;
    }
    return true;
}

// ---------- métricas ----------
void MenuConsola::capturarMetricas() {
    const MedidorRecursos& m = red.getMedidor();
    itCap = m.getIteraciones();
    memCap = m.getBytesActuales();
    extCap = m.getLlamadasExternas();
}

void MenuConsola::mostrarMetricas() const {
    std::cout << "\n--- Consumo de recursos ---\n"
              << "Iteraciones: " << itCap << "\n"
              << "Memoria actual (bytes): " << memCap << "\n"
              << "Llamadas externas: " << extCap << "\n";
}

// ---------- sesión ----------
void MenuConsola::iniciarSesion() {
    std::string nombre = leerTexto("Usuario: ");
    std::string clave = leerTexto("Contrasena: ");
    sesion = red.autenticar(nombre, clave);
    capturarMetricas();
    if (sesion == nullptr) std::cout << "Credenciales incorrectas.\n";
    mostrarMetricas();
}

void MenuConsola::mostrarAmigosDestacados() {
    int ids[3];
    int n = red.obtenerMejoresAmigos(sesion->getId(), ids, 3);
    capturarMetricas();
    std::cout << "\n";
    for (int i = 0; i < n; ++i) {
        const Usuario* amigo = red.buscarUsuarioPorId(ids[i]);
        if (amigo != nullptr) std::cout << "Foto perfil de amigo " << (i + 1) << ": " << amigo->getFotoPerfil() << "\n";
    }
    if (n == 0) std::cout << "(Aun no tiene amigos para mostrar)\n";
    mostrarMetricas();
}

// ---------- amistades ----------
void MenuConsola::enviarSolicitudPorNombre() {
    std::string nombre = leerTexto("Nombre de usuario destino: ");
    const Usuario* destino = red.buscarUsuarioPorNombre(nombre);
    if (destino == nullptr) { std::cout << "Ese usuario no existe.\n"; return; }
    FechaHora fecha;
    if (!leerFecha("Fecha y hora de envio (YYYY-MM-DD HH:MM:SS): ", "", fecha)) return;
    bool ok = red.enviarSolicitud(sesion->getId(), destino->getId(), fecha);
    capturarMetricas();
    std::cout << (ok ? "Solicitud enviada.\n"
                     : "No se envio: ya son amigos, ya hay una solicitud pendiente o es usted mismo.\n");
    mostrarMetricas();
}

void MenuConsola::gestionarSolicitudesRecibidas() {
    int capacidad = red.getCantidadUsuarios();
    int* ids = new int[capacidad];
    int n = red.obtenerSolicitudesPendientes(sesion->getId(), ids, capacidad);
    capturarMetricas();
    if (n == 0) {
        std::cout << "No tiene solicitudes pendientes.\n";
        mostrarMetricas();
        delete[] ids;
        return;
    }
    for (int i = 0; i < n; ++i) {
        const Usuario* u = red.buscarUsuarioPorId(ids[i]);
        std::cout << (i + 1) << ". " << (u != nullptr ? u->getNombreUsuario() : std::string("?")) << "\n";
    }
    int eleccion = leerEntero("Numero de solicitud (0 para volver): ");
    if (eleccion >= 1 && eleccion <= n) {
        std::string accion = leerTexto("A = aceptar, R = rechazar: ");
        bool ok = false;
        if (accion == "A" || accion == "a") {
            ok = red.aceptarSolicitud(sesion->getId(), ids[eleccion - 1]);
            capturarMetricas();
            std::cout << (ok ? "Ahora son amigos.\n" : "No se pudo aceptar.\n");
            mostrarMetricas();
        } else if (accion == "R" || accion == "r") {
            ok = red.rechazarSolicitud(sesion->getId(), ids[eleccion - 1]);
            capturarMetricas();
            std::cout << (ok ? "Solicitud rechazada.\n" : "No se pudo rechazar.\n");
            mostrarMetricas();
        }
    }
    delete[] ids;
}

void MenuConsola::administrarAmistades() {
    std::string op = leerTexto("\n1.- Enviar solicitud\n2.- Ver solicitudes recibidas\nOpcion: ");
    if (op == "1") enviarSolicitudPorNombre();
    else if (op == "2") gestionarSolicitudesRecibidas();
}

void MenuConsola::verSugerencias() {
    int k = leerEntero("Cuantas sugerencias (1 a 20): ");
    if (k < 1 || k > 20) { std::cout << "Valor fuera de rango.\n"; return; }
    int* ids = new int[k];
    int* comunes = new int[k];
    int n = red.obtenerSugerencias(sesion->getId(), k, ids, comunes);
    capturarMetricas();
    for (int i = 0; i < n; ++i) {
        const Usuario* u = red.buscarUsuarioPorId(ids[i]);
        std::cout << (i + 1) << ". " << (u != nullptr ? u->getNombreUsuario() : std::string("?"))
                  << " (" << comunes[i] << " amigos en comun)\n";
    }
    if (n == 0) std::cout << "No hay sugerencias.\n";
    mostrarMetricas();
    if (n > 0) {
        int eleccion = leerEntero("Numero al que desea enviar solicitud (0 para ninguno): ");
        if (eleccion >= 1 && eleccion <= n) {
            FechaHora fecha;
            if (leerFecha("Fecha y hora de envio (YYYY-MM-DD HH:MM:SS): ", "", fecha)) {
                bool ok = red.enviarSolicitud(sesion->getId(), ids[eleccion - 1], fecha);
                capturarMetricas();
                std::cout << (ok ? "Solicitud enviada.\n" : "No se envio la solicitud.\n");
                mostrarMetricas();
            }
        }
    }
    delete[] ids;
    delete[] comunes;
}

void MenuConsola::visitarPerfil() {
    // PENDIENTE (Joshua): listar albumes con obtenerAlbumesVisibles y recorrer las fotos en carrusel.
    std::cout << "Funcionalidad en desarrollo.\n";
}

// ---------- mensajes ----------
void MenuConsola::enviarMensaje() {
    std::string nombre = leerTexto("Nombre del amigo: ");
    const Usuario* destino = red.buscarUsuarioPorNombre(nombre);
    if (destino == nullptr) { std::cout << "Ese usuario no existe.\n"; return; }
    std::string texto = leerTexto("Mensaje (max 512 caracteres): ");
    FechaHora fecha;
    if (!leerFecha("Fecha y hora de envio (YYYY-MM-DD HH:MM:SS): ", "", fecha)) return;
    int codigo = red.enviarMensaje(sesion->getId(), destino->getId(), texto, fecha);
    capturarMetricas();
    switch (codigo) {
    case 0: std::cout << "Mensaje enviado.\n"; break;
    case 1: std::cout << "Destinatario invalido.\n"; break;
    case 2: std::cout << "Solo puede escribir a sus amigos.\n"; break;
    case 3: std::cout << "El mensaje no puede estar vacio.\n"; break;
    case 4: std::cout << "El mensaje supera 512 caracteres.\n"; break;
    case 5: std::cout << "Fecha invalida.\n"; break;
    default: std::cout << "No se pudo registrar el cambio.\n"; break;
    }
    mostrarMetricas();
}

void MenuConsola::leerNuevos() {
    int cantidad = sesion->getCantidadMensajesRecibidos();
    if (cantidad == 0) { std::cout << "No tiene mensajes.\n"; return; }
    Mensaje** buffer = new Mensaje*[cantidad];
    int n = red.leerNuevosMensajes(sesion->getId(), buffer, cantidad);
    capturarMetricas();
    for (int i = 0; i < n; ++i) {
        std::cout << "De " << buffer[i]->obtenerEmisor()->getNombreUsuario()
        << " [" << buffer[i]->getFechaEnvio().aTexto() << "]: "
        << buffer[i]->getTexto() << "\n";
    }
    if (n == 0) std::cout << "No tiene mensajes nuevos.\n";
    mostrarMetricas();
    delete[] buffer;
}

void MenuConsola::buscarPorFechas() {
    int cantidad = sesion->getCantidadMensajesRecibidos();
    if (cantidad == 0) { std::cout << "No tiene mensajes.\n"; return; }
    FechaHora desde, hasta;
    if (!leerFecha("Fecha inicial (YYYY-MM-DD): ", "00:00:00", desde)) return;
    if (!leerFecha("Fecha final (YYYY-MM-DD): ", "23:59:59", hasta)) return;
    const Mensaje** buffer = new const Mensaje*[cantidad];
    int n = red.buscarMensajes(sesion->getId(), desde, hasta, buffer, cantidad);
    capturarMetricas();
    for (int i = 0; i < n; ++i) {
        std::cout << "De " << buffer[i]->obtenerEmisor()->getNombreUsuario()
        << " [" << buffer[i]->getFechaEnvio().aTexto() << "]: "
        << buffer[i]->getTexto() << "\n";
    }
    if (n == 0) std::cout << "No hay mensajes en ese rango.\n";
    mostrarMetricas();
    delete[] buffer;
}

void MenuConsola::administrarMensajes() {
    std::string op = leerTexto("\n1.- Enviar mensaje\n2.- Leer nuevos mensajes\n3.- Buscar por rango de fechas\nOpcion: ");
    if (op == "1") enviarMensaje();
    else if (op == "2") leerNuevos();
    else if (op == "3") buscarPorFechas();
}

// ---------- menú principal ----------
void MenuConsola::menuPrincipal() {
    bool cambioUsado = false;     // el grupo de fotos solo se puede renovar una vez
    mostrarAmigosDestacados();
    bool activo = true;
    while (activo) {
        std::cout << "\nBienvenido a UdeaBook " << sesion->getNombreUsuario() << "\n\n"
                  << "Menu de opciones:\n"
                  << "1.- Administrar amistades\n"
                  << "2.- Visitar perfil de un amigo\n"
                  << "3.- Ver sugerencias de amistad\n"
                  << "4.- Administrar mensajes\n"
                  << ".- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n"
                  << ".- Ingrese 'S' o 's' para cerrar su sesion.\n";
        std::string op = leerTexto("Su opcion: ");
        if (op == "1") administrarAmistades();
        else if (op == "2") visitarPerfil();
        else if (op == "3") verSugerencias();
        else if (op == "4") administrarMensajes();
        else if (op == "N" || op == "n") {
            if (cambioUsado) std::cout << "El grupo de fotos solo se puede actualizar una vez.\n";
            else { cambioUsado = true; mostrarAmigosDestacados(); }
        }
        else if (op == "S" || op == "s") { sesion = nullptr; activo = false; }
        else std::cout << "Opcion invalida.\n";
    }
}

void MenuConsola::ejecutar() {
    bool seguir = true;
    while (seguir) {
        std::string op = leerTexto("\n=== UdeaBook ===\n1.- Iniciar sesion\n0.- Salir\nOpcion: ");
        if (op == "1") {
            iniciarSesion();
            if (sesion != nullptr) menuPrincipal();
        } else if (op == "0") seguir = false;
    }
}