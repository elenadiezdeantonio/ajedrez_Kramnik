#include "peon.h"
#include "tablero.h"

Peon::Peon(Color color, Posicion pos) : Pieza(color, pos) {}

char Peon::getSimbolo() const {
    if (color == Color::BLANCO) {
        return 'P';
    }
    else {
        return 'p';
    }
}

bool Peon::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
    int direccion = (color == Color::BLANCO) ? 1 : -1;
    int filaActual = posicion.fila;
    int colActual = posicion.columna;

    // Movimiento hacia adelante
    if (destino.columna == colActual && destino.fila == filaActual + direccion) {
        return tablero.obtenerPieza(destino) == nullptr;
    }

    // Captura en diagonal (puede ser incluso tu propia pieza)
    if ((destino.columna == colActual + 1 || destino.columna == colActual - 1) &&
        destino.fila == filaActual + direccion) {
        return tablero.obtenerPieza(destino) != nullptr;
    }

    return false;
}
