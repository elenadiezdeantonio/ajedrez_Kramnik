#include "juego.h"
#include "peon.h"
#include "rey.h"
#include "reina.h"
#include "torre.h"
#include "alfil.h"
#include "caballo.h"
#include "posicion.h"
#include <iostream>

Juego::Juego() : turnoActual(Color::BLANCO) {}

void Juego::iniciar() {
    for (int col = 0; col < 5; ++col) {
        tablero.colocarPieza(new Peon(Color::NEGRO, { 4, col }), { 4, col });
        tablero.colocarPieza(new Peon(Color::BLANCO, { 1, col }), { 1, col });
    }

    tablero.colocarPieza(new Rey(Color::NEGRO, { 5, 0 }), { 5, 0 });
    tablero.colocarPieza(new Reina(Color::NEGRO, { 5, 1 }), { 5, 1 });
    tablero.colocarPieza(new Alfil(Color::NEGRO, { 5, 2 }), { 5, 2 });
    tablero.colocarPieza(new Caballo(Color::NEGRO, { 5, 3 }), { 5, 3 });
    tablero.colocarPieza(new Torre(Color::NEGRO, { 5, 4 }), { 5, 4 });

    tablero.colocarPieza(new Torre(Color::BLANCO, { 0, 0 }), { 0, 0 });
    tablero.colocarPieza(new Caballo(Color::BLANCO, { 0, 1 }), { 0, 1 });
    tablero.colocarPieza(new Alfil(Color::BLANCO, { 0, 2 }), { 0, 2 });
    tablero.colocarPieza(new Reina(Color::BLANCO, { 0, 3 }), { 0, 3 });
    tablero.colocarPieza(new Rey(Color::BLANCO, { 0, 4 }), { 0, 4 });
}

bool Juego::jugarTurno(Posicion origen, Posicion destino) {
    Pieza* pieza = tablero.obtenerPieza(origen);
    if (pieza && pieza->getColor() == turnoActual && pieza->esMovimientoValido(destino, tablero)) {
        tablero.moverPieza(origen, destino);

        // Verifica si el peón debe coronarse
        Pieza* piezaMovida = tablero.obtenerPieza(destino);
        if (piezaMovida && dynamic_cast<Peon*>(piezaMovida)) {
            int filaFinal;
            if (piezaMovida->getColor() == Color::BLANCO) {
                filaFinal = 5;
            }
            else {
                filaFinal = 0;
            }

            if (destino.fila == filaFinal) {
                std::cout << "¡Tu peón ha llegado al final del tablero!\n";
                std::cout << "¿A qué pieza deseas coronarlo? (R: Reina, T: Torre, A: Alfil, C: Caballo): ";

                char opcion;
                std::cin >> opcion;
                opcion = std::toupper(opcion); // Por si meten minúsculas

                delete piezaMovida;

                switch (opcion) {
                case 'T':
                    tablero.colocarPieza(new Torre(turnoActual, destino), destino);
                    break;
                case 'A':
                    tablero.colocarPieza(new Alfil(turnoActual, destino), destino);
                    break;
                case 'C':
                    tablero.colocarPieza(new Caballo(turnoActual, destino), destino);
                    break;
                default:
                    tablero.colocarPieza(new Reina(turnoActual, destino), destino);
                    break;
                }

                std::cout << "¡Peón coronado exitosamente!\n";
            }
        }

        cambiarTurno();
        return true;
    }
    return false;
}



void Juego::mostrarTablero() const {
    tablero.mostrar();
}

Color Juego::obtenerTurnoActual() const {
    return turnoActual;
}

void Juego::cambiarTurno() {
    if (turnoActual == Color::BLANCO) {
        turnoActual = Color::NEGRO;
    }
    else {
        turnoActual = Color::BLANCO;
    }
}

