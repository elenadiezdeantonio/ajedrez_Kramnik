#include "alfil.h"
#include "Tablero.h"
#include <cmath>
Alfil::Alfil(Color color, Posicion pos) : Pieza(color, pos) {}

char Alfil::getSimbolo() const {
    if (color == Color::BLANCO) {
        return 'A';
    }
    else {
        return 'a';
    }
}

bool Alfil::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
    // Movimiento en diagonal:la fila y columna siempre aumentan igual.
    int df = std::abs(destino.fila - posicion.fila);
    int dc = std::abs(destino.columna - posicion.columna);
    return (destino != posicion) && (df == dc) && tablero.caminoLibre(posicion, destino);

}
