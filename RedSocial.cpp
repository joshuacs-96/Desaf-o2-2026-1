#include "RedSocial.h"
#include "AlgoritmosAmistad.h"
#include "UtilCsv.h"

#include <fstream>

namespace {
bool partirLinea(const std::string& linea, std::string* campos, int cantidad) {
    int campo = 0; bool entreComillas = false; std::string actual;
    for (std::size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == '"') { if (entreComillas && i + 1 < linea.size() && linea[i + 1] == '"') { actual += c; ++i; } else entreComillas = !entreComillas; }
        else if (c == ',' && !entreComillas) { if (campo >= cantidad) return false; campos[campo++] = actual; actual.clear(); }
        else actual += c;
    }
    if (entreComillas || campo != cantidad - 1) return false;
    campos[campo] = actual; return true;
}
long aLong(const std::string& texto) {
    long resultado = 0;
    bool negativo = false;
    std::size_t i = 0;
    if (i < texto.size() && texto[i] == '-') { negativo = true; ++i; }
    for (; i < texto.size(); ++i) {
        if (texto[i] < '0' || texto[i] > '9') break;
        resultado = resultado * 10 + (texto[i] - '0');
    }
    return negativo ? -resultado : resultado;
}
int aEntero(const std::string& texto) { return static_cast<int>(aLong(texto)); }
Privacidad privacidadDesdeTexto(const std::string& texto) { return texto == "publico" ? Privacidad::Publico : (texto == "amigos" ? Privacidad::Amigos : Privacidad::Privado); }
EstadoSolicitud estadoDesdeTexto(const std::string& texto) { return texto == "aceptada" ? EstadoSolicitud::Aceptada : (texto == "rechazada" ? EstadoSolicitud::Rechazada : EstadoSolicitud::Pendiente); }
}

RedSocial::RedSocial() : semilla(123456789u), recuperando(false) {}
RedSocial::~RedSocial() { for (int i = 0; i < mensajes.obtenerTam(); ++i) delete mensajes[i]; }

int RedSocial::posicionUsuarioPorId(int id, long& iteraciones) const {
    for (int i = 0; i < usuarios.obtenerTam(); ++i) { ++iteraciones; if (usuarios[i].getId() == id) return i; }
    return -1;
}
int RedSocial::posicionAlbumPorId(int id, long& iteraciones) const {
    for (int i = 0; i < usuarios.obtenerTam(); ++i) for (int j = 0; j < usuarios[i].getCantidadAlbumes(); ++j) { ++iteraciones; if (usuarios[i].getAlbum(j)->getId() == id) return i; }
    return -1;
}
Album* RedSocial::buscarAlbumPorId(int id, long& iteraciones) {
    for (int i = 0; i < usuarios.obtenerTam(); ++i) { Album* album = usuarios[i].buscarAlbumPorId(id, iteraciones); if (album != nullptr) return album; }
    return nullptr;
}
bool RedSocial::haySolicitudPendienteEntre(int idA, int idB, long& iteraciones) const {
    for (int i = 0; i < solicitudes.obtenerTam(); ++i) { ++iteraciones; const SolicitudAmistad& s = solicitudes[i]; if (s.estaPendiente() && ((s.getIdRemitente() == idA && s.getIdDestinatario() == idB) || (s.getIdRemitente() == idB && s.getIdDestinatario() == idA))) return true; }
    return false;
}

bool RedSocial::cargarUsuarios(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna();
    if (!archivo.is_open()) return false;
    for (int i = 0; i < mensajes.obtenerTam(); ++i) delete mensajes[i];
    usuarios.limpiar(); solicitudes.limpiar(); mensajes.limpiar();
    std::string linea; std::getline(archivo, linea); // encabezado
    while (std::getline(archivo, linea)) {
        std::string c[9]; FechaHora nacimiento, inscripcion;
        medidor.sumarIteraciones();
        if (!partirLinea(linea, c, 9) || !FechaHora::desdeTexto(c[2], nacimiento) || !FechaHora::desdeTexto(c[8], inscripcion)) continue;
        int id = aEntero(c[0]); long it = 0;
        if (id <= 0 || c[1].empty() || posicionUsuarioPorId(id, it) >= 0) continue;
        medidor.sumarIteraciones(it);
        usuarios.agregar(Usuario(id, usuarios.obtenerTam(), c[1], nacimiento, c[3], c[4], c[5], c[6], c[7], inscripcion));
    }
    amistades.inicializar(usuarios.obtenerTam());
    return true;
}

bool RedSocial::cargarAmistades(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) { std::string c[2]; if (!partirLinea(linea, c, 2)) continue; long it = 0; int a = posicionUsuarioPorId(aEntero(c[0]), it), b = posicionUsuarioPorId(aEntero(c[1]), it); amistades.establecerAmistad(a, b, it); medidor.sumarIteraciones(it); }
    return true;
}

bool RedSocial::cargarSolicitudes(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) { std::string c[3]; FechaHora fecha; long it = 0; if (!partirLinea(linea, c, 3) || !FechaHora::desdeTexto(c[2], fecha)) continue; int a = posicionUsuarioPorId(aEntero(c[0]), it), b = posicionUsuarioPorId(aEntero(c[1]), it); medidor.sumarIteraciones(it); if (a >= 0 && b >= 0) solicitudes.agregar(SolicitudAmistad(&usuarios[a], &usuarios[b], fecha)); }
    return true;
}

bool RedSocial::cargarAlbumes(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) { std::string c[5]; long it = 0; if (!partirLinea(linea, c, 5)) continue; int p = posicionUsuarioPorId(aEntero(c[1]), it); medidor.sumarIteraciones(it); if (p >= 0) usuarios[p].agregarAlbum(Album(aEntero(c[0]), aEntero(c[1]), c[2], c[3], privacidadDesdeTexto(c[4]))); }
    return true;
}

bool RedSocial::cargarFotos(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) { std::string c[6]; FechaHora fecha; if (!partirLinea(linea, c, 6) || !FechaHora::desdeTexto(c[2], fecha)) continue; long it = 0; Album* album = buscarAlbumPorId(aEntero(c[1]), it); medidor.sumarIteraciones(it); if (album != nullptr) album->agregarFoto(Foto(aEntero(c[0]), fecha, aLong(c[3]), c[4])); }
    return true;
}

bool RedSocial::cargarLikes(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) { std::string c[2]; if (!partirLinea(linea, c, 2)) continue; long it = 0; Foto* foto = nullptr; for (int u = 0; u < usuarios.obtenerTam() && foto == nullptr; ++u) for (int a = 0; a < usuarios[u].getCantidadAlbumes() && foto == nullptr; ++a) foto = usuarios[u].getAlbum(a)->buscarFotoPorId(aEntero(c[1]), it); medidor.sumarIteraciones(it); if (foto != nullptr) foto->agregarLikeCargado(aEntero(c[0]), it); }
    return true;
}

bool RedSocial::cargarMensajes(const std::string& ruta) {
    std::ifstream archivo(ruta.c_str()); medidor.registrarLlamadaExterna(); if (!archivo.is_open()) return false;
    std::string linea; std::getline(archivo, linea);
    while (std::getline(archivo, linea)) {
        std::string c[6]; FechaHora envio, recepcion; long it = 0;
        if (!partirLinea(linea, c, 6) || !FechaHora::desdeTexto(c[3], envio) || !FechaHora::desdeTexto(c[4], recepcion)) continue;
        int emisor = posicionUsuarioPorId(aEntero(c[0]), it), receptor = posicionUsuarioPorId(aEntero(c[1]), it); medidor.sumarIteraciones(it);
        if (emisor < 0 || receptor < 0) continue;
        Mensaje* mensaje = new Mensaje(&usuarios[emisor], &usuarios[receptor], c[2], envio, recepcion);
        mensajes.agregar(mensaje);
        usuarios[emisor].agregarMensajeEnviado(mensaje); usuarios[receptor].agregarMensajeRecibido(mensaje);
        if (aEntero(c[5]) != 0) mensaje->marcarLeido();
    }
    return true;
}

bool RedSocial::cargarDesdeDirectorio(const std::string& directorio) {
    medidor.reiniciarOperacion();
    std::string base = directorio; if (!base.empty() && base.back() != '/' && base.back() != '\\') base += "/";
    bool ok = cargarUsuarios(base + "usuarios.csv") && cargarAmistades(base + "amistades.csv") && cargarSolicitudes(base + "solicitudes.csv") && cargarAlbumes(base + "albumes.csv") && cargarFotos(base + "fotos.csv") && cargarLikes(base + "likes.csv") && cargarMensajes(base + "mensajes.csv");
    if (ok) { persistencia.configurar(base + "cambios.log"); ok = recuperarCambios(); }
    return ok;
}

const Usuario* RedSocial::buscarUsuarioPorId(int id) { medidor.reiniciarOperacion(); long it = 0; int p = posicionUsuarioPorId(id, it); medidor.sumarIteraciones(it); return p < 0 ? nullptr : &usuarios[p]; }
const Usuario* RedSocial::buscarUsuarioPorNombre(const std::string& nombre) { medidor.reiniciarOperacion(); for (int i = 0; i < usuarios.obtenerTam(); ++i) { medidor.sumarIteraciones(); if (usuarios[i].getNombreUsuario() == nombre) return &usuarios[i]; } return nullptr; }
int RedSocial::getCantidadUsuarios() const { return usuarios.obtenerTam(); }
const MedidorRecursos& RedSocial::getMedidor() const { medidor.actualizarMemoriaActual(memoriaUsada()); return medidor; }
long RedSocial::memoriaUsada() const { long total = static_cast<long>(sizeof(*this)) + static_cast<long>(usuarios.obtenerCapacidad()) * static_cast<long>(sizeof(Usuario)) + static_cast<long>(solicitudes.obtenerCapacidad()) * static_cast<long>(sizeof(SolicitudAmistad)) + static_cast<long>(mensajes.obtenerCapacidad()) * static_cast<long>(sizeof(Mensaje*)) + amistades.memoriaUsada(); for (int i = 0; i < usuarios.obtenerTam(); ++i) total += usuarios[i].memoriaUsada() - static_cast<long>(sizeof(Usuario)); for (int i = 0; i < mensajes.obtenerTam(); ++i) total += static_cast<long>(sizeof(Mensaje)) + static_cast<long>(mensajes[i]->getTexto().capacity()) + 1; return total; }

bool RedSocial::sonAmigos(int idUsuarioA, int idUsuarioB) { medidor.reiniciarOperacion(); long it = 0; int a = posicionUsuarioPorId(idUsuarioA, it), b = posicionUsuarioPorId(idUsuarioB, it); bool resultado = amistades.sonAmigos(a, b, it); medidor.sumarIteraciones(it); return resultado; }
bool RedSocial::enviarSolicitud(int idRemitente, int idDestinatario, const FechaHora& fechaHora) {
    medidor.reiniciarOperacion();
    long it = 0;
    int a = posicionUsuarioPorId(idRemitente, it);
    int b = posicionUsuarioPorId(idDestinatario, it);
    bool invalida = a < 0 || b < 0 || a == b || amistades.sonAmigos(a, b, it)
                    || haySolicitudPendienteEntre(idRemitente, idDestinatario, it);
    medidor.sumarIteraciones(it);
    if (invalida || !fechaHora.esValida()) return false;
    if (!registrarCambio("SOLICITUD," + UtilCsv::aTexto(idRemitente) + ","
                         + UtilCsv::aTexto(idDestinatario) + "," + fechaHora.aTexto())) return false;
    solicitudes.agregar(SolicitudAmistad(&usuarios[a], &usuarios[b], fechaHora));
    return true;
}
bool RedSocial::aceptarSolicitud(int idDestinatario, int idRemitente) {
    medidor.reiniciarOperacion();
    long it = 0;
    for (int i = 0; i < solicitudes.obtenerTam(); ++i) {
        ++it;
        SolicitudAmistad& s = solicitudes[i];
        if (s.estaPendiente() && s.getIdDestinatario() == idDestinatario
            && s.getIdRemitente() == idRemitente) {
            int a = posicionUsuarioPorId(idDestinatario, it);
            int b = posicionUsuarioPorId(idRemitente, it);
            if (a < 0 || b < 0) { medidor.sumarIteraciones(it); return false; }
            if (!registrarCambio("ACEPTAR," + UtilCsv::aTexto(idDestinatario) + ","
                                 + UtilCsv::aTexto(idRemitente))) { medidor.sumarIteraciones(it); return false; }
            amistades.establecerAmistad(a, b, it);
            s.aceptar();
            medidor.sumarIteraciones(it);
            return true;
        }
    }
    medidor.sumarIteraciones(it);
    return false;
}

const Usuario* RedSocial::autenticar(const std::string& nombre,
                                     const std::string& contrasena) {
    const Usuario* u = buscarUsuarioPorNombre(nombre);
    if (u == nullptr || !u->verificarContrasena(contrasena)) return nullptr;
    return u;
}

bool RedSocial::rechazarSolicitud(int idDestinatario, int idRemitente) {
    medidor.reiniciarOperacion();
    long it = 0;
    for (int i = 0; i < solicitudes.obtenerTam(); ++i) {
        ++it;
        SolicitudAmistad& s = solicitudes[i];
        if (s.estaPendiente() && s.getIdDestinatario() == idDestinatario
            && s.getIdRemitente() == idRemitente) {
            if (!registrarCambio("RECHAZAR," + UtilCsv::aTexto(idDestinatario) + ","
                                 + UtilCsv::aTexto(idRemitente))) { medidor.sumarIteraciones(it); return false; }
            s.rechazar();
            medidor.sumarIteraciones(it);
            return true;
        }
    }
    medidor.sumarIteraciones(it);
    return false;
}
int RedSocial::obtenerSolicitudesPendientes(int idDestinatario, int* idsRemitentes, int capacidad) { medidor.reiniciarOperacion(); if (idsRemitentes == nullptr || capacidad <= 0) return 0; int n = 0; for (int i = 0; i < solicitudes.obtenerTam(); ++i) { medidor.sumarIteraciones(); if (solicitudes[i].estaPendiente() && solicitudes[i].getIdDestinatario() == idDestinatario && n < capacidad) idsRemitentes[n++] = solicitudes[i].getIdRemitente(); } return n; }
int RedSocial::obtenerSugerencias(int idUsuario, int maximo, int* idsSalida, int* comunesSalida) { medidor.reiniciarOperacion(); if (idsSalida == nullptr || comunesSalida == nullptr || maximo <= 0) return 0; long it = 0; int p = posicionUsuarioPorId(idUsuario, it); if (p < 0) { medidor.sumarIteraciones(it); return 0; } int n = AlgoritmosAmistad::sugerir(amistades, usuarios[p].getIndiceMatriz(), maximo, idsSalida, comunesSalida, it); for (int i = 0; i < n; ++i) idsSalida[i] = usuarios[idsSalida[i]].getId(); medidor.sumarIteraciones(it); return n; }
int RedSocial::obtenerMejoresAmigos(int idUsuario, int* idsSalida, int capacidad) { medidor.reiniciarOperacion(); if (idsSalida == nullptr || capacidad <= 0) return 0; long it = 0; int p = posicionUsuarioPorId(idUsuario, it); if (p < 0) return 0; int total = usuarios.obtenerTam(); int* indices = new int[total]; int* comunes = new int[total]; int cantidad = amistades.obtenerAmigos(usuarios[p].getIndiceMatriz(), indices, total, it); for (int i = 0; i < cantidad; ++i) comunes[i] = amistades.contarAmigosComunes(usuarios[p].getIndiceMatriz(), indices[i], it); int n = AlgoritmosAmistad::seleccionarDestacados(indices, comunes, cantidad, idsSalida, capacidad, semilla, it); for (int i = 0; i < n; ++i) idsSalida[i] = usuarios[idsSalida[i]].getId(); delete[] indices; delete[] comunes; medidor.sumarIteraciones(it); return n; }
int RedSocial::obtenerAlbumesVisibles(int idVisitante, int idPropietario, int* idsAlbumes, int capacidad) { medidor.reiniciarOperacion(); if (idsAlbumes == nullptr || capacidad <= 0) return 0; long it = 0; int visitante = posicionUsuarioPorId(idVisitante, it), propietario = posicionUsuarioPorId(idPropietario, it); if (visitante < 0 || propietario < 0) return 0; bool amigo = amistades.sonAmigos(visitante, propietario, it); int n = 0; for (int i = 0; i < usuarios[propietario].getCantidadAlbumes(); ++i) { ++it; const Album* album = usuarios[propietario].getAlbum(i); if (album->esVisiblePara(idVisitante == idPropietario, amigo) && n < capacidad) idsAlbumes[n++] = album->getId(); } medidor.sumarIteraciones(it); return n; }
bool RedSocial::alternarLike(int idUsuario, int idFoto) {
    medidor.reiniciarOperacion();
    long it = 0;
    int usuario = posicionUsuarioPorId(idUsuario, it);
    if (usuario < 0) { medidor.sumarIteraciones(it); return false; }

    Foto* foto = nullptr;
    Album* encontrado = nullptr;
    for (int u = 0; u < usuarios.obtenerTam() && foto == nullptr; ++u)
        for (int a = 0; a < usuarios[u].getCantidadAlbumes() && foto == nullptr; ++a) {
            Album* album = usuarios[u].getAlbum(a);
            foto = album->buscarFotoPorId(idFoto, it);
            if (foto != nullptr) encontrado = album;
        }
    if (foto == nullptr || encontrado == nullptr) { medidor.sumarIteraciones(it); return false; }

    int propietario = posicionUsuarioPorId(encontrado->getIdPropietario(), it);
    bool amigo = amistades.sonAmigos(usuario, propietario, it);
    if (!encontrado->esVisiblePara(usuario == propietario, amigo)) { medidor.sumarIteraciones(it); return false; }

    // registro del cambio antes de aplicarlo en memoria
    if (!registrarCambio("LIKE," + UtilCsv::aTexto(idUsuario) + "," + UtilCsv::aTexto(idFoto))) {
        medidor.sumarIteraciones(it);
        return false;
    }

    bool agregado = foto->alternarLike(idUsuario, it);
    medidor.sumarIteraciones(it);
    return agregado;
}