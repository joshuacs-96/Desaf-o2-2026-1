#ifndef FOTO_H
#define FOTO_H

#include <string>
#include "FechaHora.h"
#include "ArregloDinamico.h"

class Foto {
private:
    int id;
    FechaHora fechaCaptura;
    long tamanoBytes;
    std::string ruta;
    ArregloDinamico<int> idsLikes;

public:
    Foto();
    Foto(int id, const FechaHora& fechaCaptura, long tamanoBytes, const std::string& ruta);
    int getId() const;
    const FechaHora& getFechaCaptura() const;
    long getTamanoBytes() const;
    const std::string& getRuta() const;
    int getCantidadLikes() const;
    bool tieneLikeDe(int idUsuario, long& iteraciones) const;
    bool alternarLike(int idUsuario, long& iteraciones);
    bool agregarLikeCargado(int idUsuario, long& iteraciones);
    long memoriaUsada() const;
};

#endif
