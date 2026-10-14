#include "FechaHora.h"

#include <cstdio>

FechaHora::FechaHora() : anio(0), mes(0), dia(0), hora(0), minuto(0), segundo(0) {}

FechaHora::FechaHora(int anio, int mes, int dia, int hora, int minuto, int segundo)
    : anio(anio), mes(mes), dia(dia), hora(hora), minuto(minuto), segundo(segundo) {}

bool FechaHora::esBisiesto(int valor) {
    return (valor % 4 == 0 && valor % 100 != 0) || valor % 400 == 0;
}

int FechaHora::diasEnMes(int valorMes, int valorAnio) {
    static const int dias[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (valorMes < 1 || valorMes > 12) return 0;
    return valorMes == 2 && esBisiesto(valorAnio) ? 29 : dias[valorMes];
}

bool FechaHora::esValida() const {
    return anio >= 1 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= diasEnMes(mes, anio) &&
           hora >= 0 && hora <= 23 && minuto >= 0 && minuto <= 59 && segundo >= 0 && segundo <= 59;
}

int FechaHora::getAnio() const { return anio; }
int FechaHora::getMes() const { return mes; }
int FechaHora::getDia() const { return dia; }
int FechaHora::getHora() const { return hora; }
int FechaHora::getMinuto() const { return minuto; }
int FechaHora::getSegundo() const { return segundo; }

std::string FechaHora::aTexto() const {
    char texto[20];
    std::snprintf(texto, sizeof(texto), "%04d-%02d-%02d %02d:%02d:%02d", anio, mes, dia, hora, minuto, segundo);
    return texto;
}

bool FechaHora::desdeTexto(const std::string& texto, FechaHora& salida) {
    int a, m, d, h, min, s;
    char adicional;
    if (std::sscanf(texto.c_str(), "%d-%d-%d %d:%d:%d%c", &a, &m, &d, &h, &min, &s, &adicional) != 6) return false;
    FechaHora fecha(a, m, d, h, min, s);
    if (!fecha.esValida()) return false;
    salida = fecha;
    return true;
}

bool FechaHora::operator==(const FechaHora& otra) const { return anio == otra.anio && mes == otra.mes && dia == otra.dia && hora == otra.hora && minuto == otra.minuto && segundo == otra.segundo; }
bool FechaHora::operator!=(const FechaHora& otra) const { return !(*this == otra); }
bool FechaHora::operator<(const FechaHora& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    if (dia != otra.dia) return dia < otra.dia;
    if (hora != otra.hora) return hora < otra.hora;
    if (minuto != otra.minuto) return minuto < otra.minuto;
    return segundo < otra.segundo;
}
bool FechaHora::operator<=(const FechaHora& otra) const { return *this < otra || *this == otra; }
bool FechaHora::operator>(const FechaHora& otra) const { return otra < *this; }
bool FechaHora::operator>=(const FechaHora& otra) const { return !(*this < otra); }
