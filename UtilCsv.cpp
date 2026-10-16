#include "UtilCsv.h"

namespace UtilCsv {

bool partirLinea(const std::string& linea, std::string* campos, int cantidad) {
    int campo = 0;
    bool entreComillas = false;
    std::string actual;
    for (std::size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == '"') {
            if (entreComillas && i + 1 < linea.size() && linea[i + 1] == '"') { actual += c; ++i; }
            else entreComillas = !entreComillas;
        } else if (c == ',' && !entreComillas) {
            if (campo >= cantidad) return false;
            campos[campo++] = actual;
            actual.clear();
        } else {
            actual += c;
        }
    }
    if (entreComillas || campo != cantidad - 1) return false;
    campos[campo] = actual;
    return true;
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

std::string aTexto(long valor) {
    if (valor == 0) return "0";
    bool negativo = valor < 0;
    if (negativo) valor = -valor;
    char buffer[24];
    int n = 0;
    while (valor > 0) { buffer[n++] = static_cast<char>('0' + valor % 10); valor /= 10; }
    std::string r;
    if (negativo) r += '-';
    for (int i = n - 1; i >= 0; --i) r += buffer[i];
    return r;
}

std::string escapar(const std::string& texto) {
    std::string r = "\"";
    for (std::size_t i = 0; i < texto.size(); ++i) {
        char c = texto[i];
        if (c == '\n' || c == '\r') c = ' ';
        if (c == '"') r += '"';
        r += c;
    }
    r += '"';
    return r;
}

}