#include "RedSocial.h"

int RedSocial::enviarMensaje(int idRemitente, int idDestinatario,
                             const std::string& texto, const FechaHora& fecha) {
    medidor.reiniciarOperacion();
    long it = 0;
    int codigo = 0;
    int a = posicionUsuarioPorId(idRemitente, it);
    int b = posicionUsuarioPorId(idDestinatario, it);

    if (a < 0 || b < 0 || a == b) codigo = 1;
    else if (!amistades.sonAmigos(a, b, it)) codigo = 2;
    else if (texto.size() > 512) codigo = 4;
    else if (!fecha.esValida()) codigo = 5;
    else {
        bool hayContenido = false;               // rechaza vacío o solo espacios
        for (std::size_t i = 0; i < texto.size(); ++i) {
            ++it;
            if (texto[i] != ' ' && texto[i] != '\t') { hayContenido = true; break; }
        }
        if (!hayContenido) codigo = 3;
    }

    if (codigo == 0) {
        // un único dueño: RedSocial; las bandejas solo guardan el puntero
        Mensaje* m = new Mensaje(&usuarios[a], &usuarios[b], texto, fecha, fecha);
        mensajes.agregar(m);
        usuarios[a].agregarMensajeEnviado(m);
        usuarios[b].agregarMensajeRecibido(m);
    }
    medidor.sumarIteraciones(it);
    return codigo;
}

int RedSocial::leerNuevosMensajes(int idUsuario, Mensaje** salida, int capacidad) {
    medidor.reiniciarOperacion();
    long it = 0;
    int n = 0;
    int p = posicionUsuarioPorId(idUsuario, it);
    if (p >= 0 && salida != nullptr && capacidad > 0) {
        Usuario& u = usuarios[p];
        // 1. reunir los no leídos
        for (int i = 0; i < u.getCantidadMensajesRecibidos() && n < capacidad; ++i) {
            ++it;
            Mensaje* m = u.getMensajeRecibido(i);
            if (m != nullptr && !m->estaLeido()) salida[n++] = m;
        }
        // 2. ordenar por fecha de envío (inserción; n suele ser pequeño)
        for (int i = 1; i < n; ++i) {
            Mensaje* actual = salida[i];
            int j = i - 1;
            while (j >= 0 && actual->getFechaEnvio() < salida[j]->getFechaEnvio()) {
                ++it;
                salida[j + 1] = salida[j];
                --j;
            }
            salida[j + 1] = actual;
        }
        // 3. marcarlos como leídos; el menú los muestra desde 'salida'
        for (int i = 0; i < n; ++i) { ++it; salida[i]->marcarLeido(); }
    }
    medidor.sumarIteraciones(it);
    return n;
}

int RedSocial::buscarMensajes(int idUsuario, const FechaHora& desde, const FechaHora& hasta,
                              const Mensaje** salida, int capacidad) {
    medidor.reiniciarOperacion();
    long it = 0;
    int n = 0;
    int p = posicionUsuarioPorId(idUsuario, it);
    if (p >= 0 && salida != nullptr && capacidad > 0 && desde.esValida() && hasta.esValida()) {
        const Usuario& u = usuarios[p];
        for (int i = 0; i < u.getCantidadMensajesRecibidos() && n < capacidad; ++i) {
            ++it;
            const Mensaje* m = u.getMensajeRecibido(i);
            // ambos extremos incluidos
            if (m != nullptr && !(m->getFechaEnvio() < desde) && !(hasta < m->getFechaEnvio()))
                salida[n++] = m;
        }
    }
    medidor.sumarIteraciones(it);
    return n;
}