#ifndef FECHAHORA_H
#define FECHAHORA_H

#include <string>

class FechaHora {
private:
    int anio;
    int mes;
    int dia;
    int hora;
    int minuto;
    int segundo;

    static bool esBisiesto(int anio);
    static int diasEnMes(int mes, int anio);

public:
    FechaHora();
    FechaHora(int anio, int mes, int dia, int hora = 0, int minuto = 0, int segundo = 0);

    bool esValida() const;
    int getAnio() const;
    int getMes() const;
    int getDia() const;
    int getHora() const;
    int getMinuto() const;
    int getSegundo() const;

    std::string aTexto() const;
    static bool desdeTexto(const std::string& texto, FechaHora& salida);

    bool operator==(const FechaHora& otra) const;
    bool operator!=(const FechaHora& otra) const;
    bool operator<(const FechaHora& otra) const;
    bool operator<=(const FechaHora& otra) const;
    bool operator>(const FechaHora& otra) const;
    bool operator>=(const FechaHora& otra) const;
};

#endif
