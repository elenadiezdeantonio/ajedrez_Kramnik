#include "tablero.h"
#include "rey.h"
#include "reina.h"
#include "torre.h"
#include "alfil.h"
#include "caballo.h"
#include "peon.h"
#include <iostream>

int main() {
    Tablero tablero;

    // Colocar piezas negras (fila 5)
    tablero.colocarPieza(new Rey(Color::NEGRO, { 5, 0 }), { 5, 0 });
    tablero.colocarPieza(new Reina(Color::NEGRO, { 5, 1 }), { 5, 1 });
    tablero.colocarPieza(new Alfil(Color::NEGRO, { 5, 2 }), { 5, 2 });
    tablero.colocarPieza(new Caballo(Color::NEGRO, { 5, 3 }), { 5, 3 });
    tablero.colocarPieza(new Torre(Color::NEGRO, { 5, 4 }), { 5, 4 });

    // Peones negros (fila 4)
    for (int col = 0; col < 5; ++col) {
        tablero.colocarPieza(new Peon(Color::NEGRO, { 4, col }), { 4, col });
    }

    // Peones blancos (fila 2)
    for (int col = 0; col < 5; ++col) {
        tablero.colocarPieza(new Peon(Color::BLANCO, { 1, col }), { 1, col });
    }

    // Colocar piezas blancas (fila 1)
    tablero.colocarPieza(new Torre(Color::BLANCO, { 0, 0 }), { 0, 0 });
    tablero.colocarPieza(new Caballo(Color::BLANCO, { 0, 1 }), { 0, 1 });
    tablero.colocarPieza(new Alfil(Color::BLANCO, { 0, 2 }), { 0, 2 });
    tablero.colocarPieza(new Reina(Color::BLANCO, { 0, 3 }), { 0, 3 });
    tablero.colocarPieza(new Rey(Color::BLANCO, { 0, 4 }), { 0, 4 });

    tablero.mostrar();
    return 0;
}



