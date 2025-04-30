
#include "tablero.h"
#include "rey.h"
#include <iostream>

using namespace std;
int main() {
    Tablero tablero;

    // Colocar piezas en el tablero
    Pieza* reyBlanco = new Rey(Color::BLANCO, Posicion(0, 0));  // Colocamos un rey en la posición (0, 0)
    tablero.colocarPieza(reyBlanco, Posicion(0, 0));

    // Mostrar el tablero
    tablero.mostrar();

    // Espera para que la consola no se cierre de inmediato
    std::cin.get();
    return 0;
}
