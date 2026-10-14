#include "FechaHora.h"

#include <string>
namespace {
// agrega 'valor' con al menos 'digitos' cifras, rellenando con ceros
void agregarNumero(std::string& destino, int valor, int digitos) {
    char buffer[12];
    int n = 0;
    if (valor < 0) valor = 0;
    do { buffer[n++] = static_cast<char>('0' + valor % 10); valor /= 10; } while (valor > 0);
    while (n < digitos) buffer[n++] = '0';
    for (int i = n - 1; i >= 0; --i) destino += buffer[i];
}

// lee 'cantidad' dígitos desde 'inicio'; false si hay algún carácter no numérico
bool leerNumero(const std::string& t, int inicio, int cantidad, int& salida) {
    int r = 0;
    for (int i = inicio; i < inicio + cantidad; ++i) {
        if (t[i] < '0' || t[i] > '9') return false;
        r = r * 10 + (t[i] - '0');
    }
    salida = r;
    return true;
}
}

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
    std::string r;
    agregarNumero(r, anio, 4);    r += '-';
    agregarNumero(r, mes, 2);     r += '-';
    agregarNumero(r, dia, 2);     r += ' ';
    agregarNumero(r, hora, 2);    r += ':';
    agregarNumero(r, minuto, 2);  r += ':';
    agregarNumero(r, segundo, 2);
    return r;
}

bool FechaHora::desdeTexto(const std::string& texto, FechaHora& salida) {
    // formato exacto: YYYY-MM-DD HH:MM:SS (19 caracteres)
    if (texto.size() != 19) return false;
    if (texto[4] != '-' || texto[7] != '-' || texto[10] != ' ' ||
        texto[13] != ':' || texto[16] != ':') return false;
    int a, m, d, h, min, s;
    if (!leerNumero(texto, 0, 4, a) || !leerNumero(texto, 5, 2, m) ||
        !leerNumero(texto, 8, 2, d) || !leerNumero(texto, 11, 2, h) ||
        !leerNumero(texto, 14, 2, min) || !leerNumero(texto, 17, 2, s)) return false;
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
