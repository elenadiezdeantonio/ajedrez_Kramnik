#include <iostream>
#include "tablero.h"
#include "rey.h"
#include "reina.h"
#include "color.h"

int main() {
    // Crear el tablero de 6x5
    Tablero tablero;

    // Colocar las piezas
    tablero.colocarPieza(new Rey(Color::NEGRO, Posicion(5, 0)), Posicion(5, 0)); // Rey negro en (5,0)
    tablero.colocarPieza(new Rey(Color::BLANCO, Posicion(0, 4)), Posicion(0, 4)); // Rey blanco en (0,4)
    tablero.colocarPieza(new Reina(Color::BLANCO, Posicion(0, 3)), Posicion(0, 3)); // Reina blanca en (0,3)
    tablero.colocarPieza(new Reina(Color::NEGRO, Posicion(5, 1)), Posicion(5, 1)); // Reina negra en (5,1)

    // Mostrar el tablero inicial
    std::cout << "Tablero inicial:\n";
    tablero.mostrar();

    // Mover el rey NEGRO de (5, 0) a (4, 1)
    std::cout << "\nMover el rey negro de (5,0) a (4,1):\n";
    tablero.moverPieza(Posicion(5, 0), Posicion(4, 1));
    tablero.mostrar();

    // Mover el rey BLANCO de (0, 4) a (1, 4)
    std::cout << "\nMover el rey blanco de (0,4) a (1,4):\n";
    tablero.moverPieza(Posicion(0, 4), Posicion(1, 4));
    tablero.mostrar();

    // Mover la reina BLANCA de (0, 3) a (3, 0)
    std::cout << "\nMover la reina blanca de (0,3) a (3,0):\n";
    tablero.moverPieza(Posicion(0, 3), Posicion(3, 0));
    tablero.mostrar();

    // Mover la reina NEGRA de (5, 1) a (0, 1)
    std::cout << "\nMover la reina negra de (5,1) a (0,1):\n";
    tablero.moverPieza(Posicion(5, 1), Posicion(0, 1));
    tablero.mostrar();

    return 0;
}


