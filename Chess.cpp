#include "juego.h"
#include <iostream>

int main() {
    Juego juego;
    juego.iniciar(); // Coloca todas las piezas

    while (true) {
        juego.mostrarTablero();

        // Verificación de jaque
        if (juego.estaEnJaque(juego.obtenerTurnoActual())) {
            std::cout << "El jugador "
                << (juego.obtenerTurnoActual() == Color::BLANCO ? "blanco" : "negro")
                << " está en jaque!\n";
        }

        // Verificación de jaque mate
        if (juego.esJaqueMate(juego.obtenerTurnoActual())) {
            std::cout << "\nJAQUE MATE!\n";
            std::cout << "El jugador "
                << (juego.obtenerTurnoActual() == Color::BLANCO ? "negro" : "blanco")
                << " gana la partida.\n";
            break; // Terminar el juego
        }

        // Detectar ahogado
        if (juego.estaAhogado(juego.obtenerTurnoActual())) {
            std::cout << "¡Tablas por ahogado!\n";
            break;
        }

        // Verificación de empate por material insuficiente
        if (!juego.tieneMaterialSuficiente()) {
            std::cout << "Empate por material insuficiente.\n";
            break;
        }

        int oFila, oCol, dFila, dCol;
        std::cout << "\nTurno de " << (juego.obtenerTurnoActual() == Color::BLANCO ? "blanco" : "negro") << "\n";
        std::cout << "Ingresa origen (fila columna): ";
        std::cin >> oFila >> oCol;
        std::cout << "Ingresa destino (fila columna): ";
        std::cin >> dFila >> dCol;

        Posicion origen(oFila, oCol);
        Posicion destino(dFila, dCol);

        if (!juego.jugarTurno(origen, destino)) {
            std::cout << "\nMovimiento invalido. Intenta de nuevo.\n";
        }
    }

    return 0;
}







