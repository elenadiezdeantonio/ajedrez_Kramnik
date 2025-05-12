#include "caballo.h"
#include <cmath>
Caballo::Caballo(Color color, Posicion pos) : Pieza(color, pos) {}

char Caballo::getSimbolo() const {
    if (color == Color::BLANCO) {
        return 'C';
    }
    else {
        return 'c';
    }
}

bool Caballo::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
    int dx = std::abs(destino.columna - posicion.columna);
    int dy = std::abs(destino.fila - posicion.fila);
    // Movimiento en L
    return destino != posicion && ((dx == 2 && dy == 1) || (dx == 1 && dy == 2));

}
