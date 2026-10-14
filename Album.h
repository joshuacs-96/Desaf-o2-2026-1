#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "ArregloDinamico.h"
#include "Foto.h"

enum class Privacidad { Publico, Amigos, Privado };

class Album {
private:
    int id;
    int idPropietario;
    std::string nombre;
    std::string rutaAlmacenamiento;
    Privacidad privacidad;
    ArregloDinamico<Foto> fotos;

public:
    Album();
    Album(int id, int idPropietario, const std::string& nombre,
          const std::string& rutaAlmacenamiento, Privacidad privacidad);
    int getId() const;
    int getIdPropietario() const;
    const std::string& getNombre() const;
    const std::string& getRutaAlmacenamiento() const;
    Privacidad getPrivacidad() const;
    int getCantidadFotos() const;
    const Foto* getFoto(int posicion) const;
    Foto* buscarFotoPorId(int idFoto, long& iteraciones);
    bool agregarFoto(const Foto& foto);
    bool esVisiblePara(bool esPropietario, bool esAmigo) const;
    long memoriaUsada() const;
};

#endif
