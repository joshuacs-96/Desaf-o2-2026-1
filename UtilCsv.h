#ifndef UTILCSV_H
#define UTILCSV_H

#include <string>

namespace UtilCsv {
bool partirLinea(const std::string& linea, std::string* campos, int cantidad);
long aLong(const std::string& texto);
int aEntero(const std::string& texto);
std::string aTexto(long valor);
std::string escapar(const std::string& texto);
}

#endif