#include "MedidorRecursos.h"

MedidorRecursos::MedidorRecursos() : iteraciones(0), llamadasExternas(0), bytesActuales(0) {}
void MedidorRecursos::reiniciarOperacion() { iteraciones = 0; llamadasExternas = 0; }
void MedidorRecursos::sumarIteraciones(long cantidad) { if (cantidad > 0) iteraciones += cantidad; }
void MedidorRecursos::registrarLlamadaExterna(long cantidad) { if (cantidad > 0) llamadasExternas += cantidad; }
void MedidorRecursos::actualizarMemoriaActual(long bytes) { bytesActuales = bytes < 0 ? 0 : bytes; }
long MedidorRecursos::getIteraciones() const { return iteraciones; }
long MedidorRecursos::getLlamadasExternas() const { return llamadasExternas; }
long MedidorRecursos::getBytesActuales() const { return bytesActuales; }
