#include "torre.h"
#include "Tablero.h"

Torre::Torre(Color color, Posicion pos) : Pieza(color, pos) {}

char Torre::getSimbolo() const {
    if (color == Color::BLANCO) {
        return 'T';
    }
    else {
        return 't';
    }
}

bool Torre::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
    // Movimiento en línea recta: misma fila o misma columna
    return(destino.fila == posicion.fila || destino.columna == posicion.columna) && tablero.caminoLibre(posicion, destino) && destino != posicion;

}