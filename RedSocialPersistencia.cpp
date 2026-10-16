#include "RedSocial.h"
#include "UtilCsv.h"

bool RedSocial::registrarCambio(const std::string& linea) {
    if (recuperando) return true;               // al reaplicar no se vuelve a escribir
    bool ok = persistencia.agregarLinea(linea);
    medidor.registrarLlamadaExterna();
    return ok;
}

bool RedSocial::recuperarCambios() {
    medidor.registrarLlamadaExterna();
    if (!persistencia.abrirLectura()) return true;   // primera ejecución: no hay registro

    recuperando = true;
    std::string linea;
    while (persistencia.leerLinea(linea)) {
        medidor.sumarIteraciones();

        std::string cmd;                              // comando: texto hasta la primera coma
        std::size_t k = 0;
        while (k < linea.size() && linea[k] != ',') cmd += linea[k++];

        int n = 0;                                    // campos esperados, contando comando y FIN
        if (cmd == "SOLICITUD" || cmd == "LEIDO") n = 5;
        else if (cmd == "ACEPTAR" || cmd == "RECHAZAR" || cmd == "LIKE") n = 4;
        else if (cmd == "MENSAJE") n = 6;
        else continue;

        std::string c[6];
        if (!UtilCsv::partirLinea(linea, c, n) || c[n - 1] != "FIN") continue;  // incompleta

        if (cmd == "SOLICITUD") {
            FechaHora f;
            if (FechaHora::desdeTexto(c[3], f))
                enviarSolicitud(UtilCsv::aEntero(c[1]), UtilCsv::aEntero(c[2]), f);
        } else if (cmd == "ACEPTAR") {
            aceptarSolicitud(UtilCsv::aEntero(c[1]), UtilCsv::aEntero(c[2]));
        } else if (cmd == "RECHAZAR") {
            rechazarSolicitud(UtilCsv::aEntero(c[1]), UtilCsv::aEntero(c[2]));
        } else if (cmd == "MENSAJE") {
            FechaHora f;
            if (FechaHora::desdeTexto(c[3], f))
                enviarMensaje(UtilCsv::aEntero(c[1]), UtilCsv::aEntero(c[2]), c[4], f);
        } else if (cmd == "LIKE") {
            alternarLike(UtilCsv::aEntero(c[1]), UtilCsv::aEntero(c[2]));
        } else if (cmd == "LEIDO") {
            FechaHora f;
            if (FechaHora::desdeTexto(c[3], f)) {
                long it = 0;
                int emisor = UtilCsv::aEntero(c[1]);
                int p = posicionUsuarioPorId(UtilCsv::aEntero(c[2]), it);
                if (p >= 0) {
                    for (int j = 0; j < usuarios[p].getCantidadMensajesRecibidos(); ++j) {
                        ++it;
                        Mensaje* m = usuarios[p].getMensajeRecibido(j);
                        if (m != nullptr && !m->estaLeido() && m->obtenerEmisor()->getId() == emisor
                            && m->getFechaEnvio() == f) { m->marcarLeido(); break; }
                    }
                }
                medidor.sumarIteraciones(it);
            }
        }
    }
    persistencia.cerrarLectura();
    recuperando = false;
    medidor.reiniciarOperacion();   // las métricas de la carga no incluyen la recuperación
    return true;
}