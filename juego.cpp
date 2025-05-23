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

void Juego::iniciar5x6() {
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

void Juego::iniciarPetty() {
    for (int col = 0; col < 5; ++col) {
        tablero.colocarPieza(new Peon(Color::NEGRO, { 4, col }), { 4, col });
        tablero.colocarPieza(new Peon(Color::BLANCO, { 1, col }), { 1, col });
    }

    tablero.colocarPieza(new Reina(Color::NEGRO, { 5, 0 }), { 5, 0 });
    tablero.colocarPieza(new Rey(Color::NEGRO, { 5, 1 }), { 5, 1 });
    tablero.colocarPieza(new Alfil(Color::NEGRO, { 5, 2 }), { 5, 2 });
    tablero.colocarPieza(new Caballo(Color::NEGRO, { 5, 3 }), { 5, 3 });
    tablero.colocarPieza(new Torre(Color::NEGRO, { 5, 4 }), { 5, 4 });

    tablero.colocarPieza(new Reina(Color::BLANCO, { 0, 0 }), { 0, 0 });
    tablero.colocarPieza(new Rey(Color::BLANCO, { 0, 1 }), { 0, 1 });
    tablero.colocarPieza(new Alfil(Color::BLANCO, { 0, 2 }), { 0, 2 });
    tablero.colocarPieza(new Caballo(Color::BLANCO, { 0, 3 }), { 0, 3 });
    tablero.colocarPieza(new Torre(Color::BLANCO, { 0, 4 }), { 0, 4 });
}



bool Juego::jugarTurno(Posicion origen, Posicion destino) {
    Pieza* pieza = tablero.obtenerPieza(origen);
    if (pieza && pieza->getColor() == turnoActual && pieza->esMovimientoValido(destino, tablero)) {

        // Prohibir capturar cualquier rey
        Pieza* piezaDestino = tablero.obtenerPieza(destino);
        if (piezaDestino) {
            Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
            if (reyDestino) {
                std::cout << "\nMovimiento invalido: �No puedes capturar un rey!\n";
                return false;
            }
        }

        // Simulamos el movimiento
        Pieza* piezaCapturada = tablero.obtenerPieza(destino);
        tablero.moverPiezaSimulacion(origen, destino);

        // Verificar si despu�s de mover sigo en jaque
        bool sigoEnJaque = estaEnJaque(turnoActual);

        if (sigoEnJaque) {
            // Revertimos el movimiento
            tablero.moverPiezaSimulacion(destino, origen);
            tablero.colocarPieza(piezaCapturada, destino);

            std::cout << "\nMovimiento invalido: �Debes salir del jaque!\n";
            return false;
        }

        // Movimiento v�lido confirmado: ahora mover pieza real
        tablero.moverPiezaSimulacion(origen, destino);

        // Actualizamos contador de movimientos sin captura ni movimiento de pe�n
        bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
        bool capturaPieza = piezaDestino != nullptr;
        if (muevePeon || capturaPieza) {
            movimientosSinCapturaNiPeon = 0; // Reiniciar contador
        }
        else {
            movimientosSinCapturaNiPeon++;
        }

        // Verifica si el pe�n debe coronarse
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
                std::cout << "�Tu peon ha llegado al final del tablero!\n";
                std::cout << "�A qu� pieza deseas coronarlo? (R: Reina, T: Torre, A: Alfil, C: Caballo): ";

                char opcion;
                std::cin >> opcion;
                opcion = std::toupper(opcion); // Por si meten min�sculas

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

                std::cout << "�Peon coronado exitosamente!\n";
            }
        }

        cambiarTurno();
        registrarEstadoTablero();

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

bool Juego::estaEnJaque(Color color) {
    Posicion reyPos;

    // Buscar al rey del color especificado
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Pieza* pieza = tablero.obtenerPieza({ fila, col });
            if (pieza && pieza->getColor() == color) {
                Rey* rey = dynamic_cast<Rey*>(pieza);
                if (rey) {
                    reyPos = { fila, col };
                    break;
                }
            }
        }
    }
    // Si no se encontr� el rey, no hay jaque
    if (reyPos.fila == -1) return false;

    // Verificar si alguna pieza enemiga puede atacar al rey
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Pieza* atacante = tablero.obtenerPieza({ fila, col });
            if (atacante && atacante->getColor() != color) {
                if (atacante->esMovimientoValido(reyPos, tablero)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Juego::esJaqueMate(Color color) {
    // Para cada pieza del color dado
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == color) {
                // Intentamos moverla a todas las posiciones posibles
                for (int f2 = 0; f2 < 6; ++f2) {
                    for (int c2 = 0; c2 < 5; ++c2) {
                        Posicion destino(f2, c2);

                        // Verifica si ser�a un movimiento v�lido
                        Pieza* piezaDestino = tablero.obtenerPieza(destino);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            // Prohibimos capturar reyes
                            Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
                            if (reyDestino) {
                                continue;
                            }

                            // Simular el movimiento
                            Pieza* piezaCapturada = tablero.obtenerPieza(destino);
                            tablero.moverPiezaSimulacion(origen, destino);

                            bool sigueEnJaque = estaEnJaque(color);

                            // Revertir el movimiento
                            tablero.moverPiezaSimulacion(destino, origen);
                            tablero.colocarPieza(piezaCapturada, destino);

                            if (!sigueEnJaque) {
                                // Encontramos un movimiento que evita el jaque, no es jaque mate
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }
    //Si esta ahogado no hay jaque mate
    bool sigueAhogado = estaAhogado(color);
    if (sigueAhogado) {
        return false;
    }
    // Si no hay forma de salir del jaque
    return true;
}


bool Juego::estaAhogado(Color color) {
    if (estaEnJaque(color)) {
        return false; // Si est� en jaque, no es ahogado
    }

    // Buscar si existe al menos un movimiento legal
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == color) {
                for (int f2 = 0; f2 < 6; ++f2) {
                    for (int c2 = 0; c2 < 5; ++c2) {
                        Posicion destino(f2, c2);
                        Pieza* piezaDestino = tablero.obtenerPieza(destino);

                        // Prohibimos capturar reyes
                        Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
                        if (reyDestino) continue;

                        if (pieza->esMovimientoValido(destino, tablero)) {
                            // Simular movimiento
                            Pieza* capturada = tablero.obtenerPieza(destino);
                            tablero.moverPiezaSimulacion(origen, destino);
                            bool sigueEnJaque = estaEnJaque(color);
                            tablero.moverPiezaSimulacion(destino, origen);
                            tablero.colocarPieza(capturada, destino);

                            if (!sigueEnJaque) {
                                return false; // Hay un movimiento legal
                            }
                        }
                    }
                }
            }
        }
    }

    return true; // No hay movimientos legales -> ahogado
}
bool Juego::tieneMaterialSuficiente() {
    int reyes = 0;
    int alfiles = 0;
    int caballos = 0;
    int torres = 0;
    int reinas = 0;
    int peones = 0;

    // Contamos todas las piezas en el tablero sin tener en cuenta el color
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Posicion pos(fila, col);
            Pieza* pieza = tablero.obtenerPieza(pos);
            if (pieza) {
                if (dynamic_cast<Rey*>(pieza)) {
                    reyes++;
                }
                else if (dynamic_cast<Alfil*>(pieza)) {
                    alfiles++;
                }
                else if (dynamic_cast<Caballo*>(pieza)) {
                    caballos++;
                }
                else if (dynamic_cast<Torre*>(pieza)) {
                    torres++;
                }
                else if (dynamic_cast<Reina*>(pieza)) {
                    reinas++;
                }
                else if (dynamic_cast<Peon*>(pieza)) {
                    peones++;
                }
            }
        }
    }

    if (reinas > 0 || torres > 0 || peones > 0) {
        return true;
    }
    else if (reyes == 2 && (alfiles <= 1 || caballos <= 1) && alfiles != caballos) {
        return false;
    }
}

std::string Juego::serializarTablero() const {
    std::string estado;
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Pieza* pieza = tablero.obtenerPieza(Posicion(fila, col));
            if (pieza) {
                estado += pieza->getSimbolo();
            }
            else {
                estado += '.';
            }
        }
    }
    estado += (obtenerTurnoActual() == Color::BLANCO ? 'B' : 'N'); // A�adimos turno actual para diferenciar
    return estado;
}

void Juego::registrarEstadoTablero() {
    std::string estado = serializarTablero();
    historialTableros[estado]++;
}

bool Juego::hayTripleRepeticion() const {
    for (const auto& par : historialTableros) {
        if (par.second >= 3) {
            return true;
        }
    }
    return false;
}

bool Juego::jugarTurnoBotNoob() {
    std::srand(std::time(nullptr));
    std::vector<std::pair<Posicion, Posicion>> movimientosValidos;

    // Recorremos el tablero en busca de piezas del bot
    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == turnoActual) {
                // Probar todos los posibles destinos
                for (int f = 0; f < 6; ++f) {
                    for (int c = 0; c < 5; ++c) {
                        Posicion destino(f, c);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            // Verificamos que no se intente capturar un rey
                            Pieza* piezaDestino = tablero.obtenerPieza(destino);
                            if (piezaDestino && dynamic_cast<Rey*>(piezaDestino)) {
                                continue; // no permitimos capturar al rey
                            }

                            // Simulamos el movimiento
                            Pieza* capturada = tablero.obtenerPieza(destino);
                            tablero.moverPiezaSimulacion(origen, destino);
                            bool enJaque = estaEnJaque(turnoActual);
                            tablero.moverPiezaSimulacion(destino, origen);
                            tablero.colocarPieza(capturada, destino);

                            if (!enJaque) {
                                movimientosValidos.push_back({ origen, destino });
                            }
                        }
                    }
                }
            }
        }
    }

    if (movimientosValidos.empty()) {
        std::cout << "El bot no tiene movimientos legales disponibles.\n";
        return false;
    }

    // Elegimos uno al azar
    int idx = std::rand() % movimientosValidos.size();
    Posicion origen = movimientosValidos[idx].first;
    Posicion destino = movimientosValidos[idx].second;

    Pieza* pieza = tablero.obtenerPieza(origen);
    Pieza* piezaCapturada = tablero.obtenerPieza(destino);

    tablero.moverPiezaSimulacion(origen, destino);

    bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
    bool capturaPieza = piezaCapturada != nullptr;
    if (muevePeon || capturaPieza) {
        movimientosSinCapturaNiPeon = 0;
    }
    else {
        movimientosSinCapturaNiPeon++;
    }

    // Verifica si el pe�n debe coronarse
    Pieza* piezaMovida = tablero.obtenerPieza(destino);
    if (piezaMovida && dynamic_cast<Peon*>(piezaMovida)) {
        int filaFinal = (piezaMovida->getColor() == Color::BLANCO) ? 5 : 0;
        if (destino.fila == filaFinal) {
            delete piezaMovida;
            tablero.colocarPieza(new Reina(turnoActual, destino), destino); // Por defecto, Reina
        }
    }

    cambiarTurno();
    registrarEstadoTablero();

    return true;
}

int Juego::evaluarMovimiento(const Posicion& origen, const Posicion& destino) {
    Pieza* pieza = tablero.obtenerPieza(origen);
    Pieza* objetivo = tablero.obtenerPieza(destino);
    int puntuacion = 0;

    if (objetivo) {
        if (objetivo->getColor() == turnoActual) {
            puntuacion -= objetivo->getValor(); // castiga comerse piezas propias
        }
        else {
            puntuacion += objetivo->getValor(); // premia capturar enemigas
        }
    }

    // Bonus por coronaci�n
    if (pieza && dynamic_cast<Peon*>(pieza)) {
        int filaFinal = (pieza->getColor() == Color::BLANCO) ? 5 : 0;
        if (destino.fila == filaFinal) {
            puntuacion += 9; // Valor heur�stico de coronar a Reina
        }
    }

    // Simula el movimiento
    tablero.moverPiezaSimulacion(origen, destino);
    if (!estaEnJaque(turnoActual)) {
        puntuacion += 10; // bonus por seguridad del rey
    }
    else {
        puntuacion -= 50; // penalizaci�n si deja al rey en jaque
    }
    tablero.moverPiezaSimulacion(destino, origen);
    tablero.colocarPieza(objetivo, destino);

    return puntuacion;
}


bool Juego::jugarTurnoBotMid() {
    std::srand(std::time(nullptr));
    std::vector<std::pair<Posicion, Posicion>> mejoresMovimientos;
    int mejorPuntaje = -100000;

    for (int fila = 0; fila < 6; ++fila) {
        for (int col = 0; col < 5; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == turnoActual) {
                for (int f = 0; f < 6; ++f) {
                    for (int c = 0; c < 5; ++c) {
                        Posicion destino(f, c);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            Pieza* objetivo = tablero.obtenerPieza(destino);
                            if (objetivo && dynamic_cast<Rey*>(objetivo)) continue;

                            int score = evaluarMovimiento(origen, destino);

                            if (score > mejorPuntaje) {
                                mejoresMovimientos.clear();
                                mejoresMovimientos.emplace_back(origen, destino);
                                mejorPuntaje = score;
                            }
                            else if (score == mejorPuntaje) {
                                mejoresMovimientos.emplace_back(origen, destino);
                            }
                        }
                    }
                }
            }
        }
    }

    if (mejoresMovimientos.empty()) {
        std::cout << "El bot no tiene movimientos legales disponibles.\n";
        return false;
    }

    // Elegimos uno al azar
    int idx = std::rand() % mejoresMovimientos.size();
    Posicion origen = mejoresMovimientos[idx].first;
    Posicion destino = mejoresMovimientos[idx].second;

    Pieza* pieza = tablero.obtenerPieza(origen);
    Pieza* piezaCapturada = tablero.obtenerPieza(destino);

    tablero.moverPiezaSimulacion(origen, destino);

    bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
    bool capturaPieza = piezaCapturada != nullptr;
    if (muevePeon || capturaPieza) {
        movimientosSinCapturaNiPeon = 0;
    }
    else {
        movimientosSinCapturaNiPeon++;
    }

    // Verifica si el pe�n debe coronarse
    Pieza* piezaMovida = tablero.obtenerPieza(destino);
    if (piezaMovida && dynamic_cast<Peon*>(piezaMovida)) {
        int filaFinal = (piezaMovida->getColor() == Color::BLANCO) ? 5 : 0;
        if (destino.fila == filaFinal) {
            delete piezaMovida;
            tablero.colocarPieza(new Reina(turnoActual, destino), destino); // Por defecto, Reina
        }
    }

    cambiarTurno();
    registrarEstadoTablero();

    return true;
}







