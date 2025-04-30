
#include "posicion.h"

Posicion::Posicion(int f, int c) : fila(f), columna(c) {}

bool Posicion::operator==(const Posicion& otra) const {
    return fila == otra.fila && columna == otra.columna;
}

bool Posicion::operator!=(const Posicion& otra) const {
    return !(*this == otra);
}