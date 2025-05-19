
#include "juego.h"
#include <iostream>

int main() {
    Juego juego;
    int modo;
    std::cout << "Elige modo de juego:\n";
    std::cout << "1. Modo 5x6 en esquinas opuestas\n";
    std::cout << "2. Modo Petty\n";
    std::cout << "Opcion: ";
    std::cin >> modo;

    if (modo == 1) {
        juego.iniciar5x6();
    }
    else if (modo == 2) {
        juego.iniciarPetty();
    }

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

        // Verificación de regla de los 50 movimientos
        if (juego.getMovimientosSinCapturaNiPeon() >= 100) {
            char respuesta;
            std::cout << "Se han completado 50 movimientos por bando sin captura ni movimiento de peon.\n";
            std::cout << "¿Deseas reclamar tablas? (s/n): ";
            std::cin >> respuesta;

            if (respuesta == 's' || respuesta == 'S') {
                std::cout << "¡Tablas por regla de los 50 movimientos!\n";
                break;
            }
            // Si responde 'n' o cualquier otra cosa, simplemente sigue la partida
        }

        // Verificación de triple repetición
        if (juego.hayTripleRepeticion()) {
            char respuesta;
            std::cout << "Se ha producido una triple repeticion de la posicion.\n";
            std::cout << "¿Deseas reclamar tablas? (s/n): ";
            std::cin >> respuesta;

            if (respuesta == 's' || respuesta == 'S') {
                std::cout << "¡Tablas por triple repetición!\n";
                break;
            }
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









