#include "juego.h"
#include "peon.h"
#include "rey.h"
#include "reina.h"
#include "torre.h"
#include "alfil.h"
#include "caballo.h"
#include "posicion.h"
#include "Constantes.h"
#include <iostream>
using namespace std;

Juego::Juego() : turnoActual(Color::BLANCO), tiempoBlanco(300), tiempoNegro(300) {


}

void Juego::iniciar5x6() {
    for (int col = 0; col < MAXC; ++col) {
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
    for (int col = 0; col < MAXC; ++col) {
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
    //VERIFICA QUE NO SE LES HAN ACABADO EL TIEMPO.
    if (estadoActual != EstadoApp::JUEGO)
        return false;

      verificarTiempoAgotado();
    if (estadoActual == EstadoApp::FIN_PARTIDA)
        return false;
    if (pieza && pieza->getColor() == turnoActual && pieza->esMovimientoValido(destino, tablero)) {

        // Prohibir capturar cualquier rey
        Pieza* piezaDestino = tablero.obtenerPieza(destino);
        if (piezaDestino) {
            //Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
            if (piezaDestino->getTipo() == t_Pieza::REY) {
                cout << "\nMovimiento invalido: No puedes capturar un rey!\n";
                return false;
            }
        }

        // Simulamos el movimiento
        // Pieza* piezaCapturada = tablero.obtenerPieza(destino);
        tablero.moverPiezaSimulacion(origen, destino);

        // Verificar si después de mover sigo en jaque
        bool sigoEnJaque = estaEnJaque(turnoActual);

        if (sigoEnJaque) {
            // Revertimos el movimiento
            tablero.moverPiezaSimulacion(destino, origen);
            tablero.colocarPieza(piezaDestino, destino);

            cout << "\nMovimiento invalido: Debes salir del jaque!\n";
            return false;
        }

        // Movimiento válido confirmado: ahora mover pieza real
        tablero.moverPiezaSimulacion(origen, destino);

        // Actualizamos contador de movimientos sin captura ni movimiento de peón
        //bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
        bool muevePeon = false;
        if (pieza->getTipo() == t_Pieza::PEON)
            muevePeon = true;

        bool capturaPieza = piezaDestino != nullptr;
        if (muevePeon || capturaPieza) {
            movimientosSinCapturaNiPeon = 0; // Reiniciar contador
        }
        else {
            movimientosSinCapturaNiPeon++;
        }

        verificarCoronacion(destino);

        cambiarTurno();
        registrarEstadoTablero();
        verificarCondicionesDeTablas(vsMaquina);
        if (esJaqueMate(turnoActual)) {
            Renderizador::mensajeEstado = "Jaque mate! Ganan " + (turnoActual == Color::BLANCO ? string("negras") : string("blancas"));
            estadoActual = EstadoApp::FIN_PARTIDA;
        }
        else if (estaAhogado(turnoActual)) {
            Renderizador::mensajeEstado = "Empate por ahogado!";
            estadoActual = EstadoApp::FIN_PARTIDA;
        }
        else if (!tieneMaterialSuficiente()) {
            Renderizador::mensajeEstado = "Empate por material insuficiente!";
            estadoActual = EstadoApp::FIN_PARTIDA;
        }


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
    auto ahora = std::chrono::steady_clock::now();
    int tiempoPasado = std::chrono::duration_cast<std::chrono::seconds>(ahora - inicioTurno).count();

    if (turnoActual == Color::BLANCO) {
        tiempoBlanco -= tiempoPasado;
        turnoActual = Color::NEGRO;
    }
    else {
        tiempoNegro -= tiempoPasado;
        turnoActual = Color::BLANCO;
    }

    inicioTurno = std::chrono::steady_clock::now();
}
bool Juego::estaEnJaque(Color color) {
    Posicion reyPos;

    // Buscar al rey del color especificado
    //Cambiar el 6 por una cte global
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Pieza* pieza = tablero.obtenerPieza({ fila, col });
            if (pieza && pieza->getColor() == color) {
                //Rey* rey = dynamic_cast<Rey*>(pieza);
                if (pieza->getTipo() == t_Pieza::REY) {
                    reyPos = { fila, col };
                    break;
                }
            }
        }
    }
    // Si no se encontró el rey, no hay jaque
    if (reyPos.fila == -1) return false;

    // Verificar si alguna pieza enemiga puede atacar al rey
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
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
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == color) {
                // Intentamos moverla a todas las posiciones posibles
                for (int f2 = 0; f2 < MAXF; ++f2) {
                    for (int c2 = 0; c2 < MAXC; ++c2) {
                        Posicion destino(f2, c2);

                        // Verifica si sería un movimiento válido
                        Pieza* piezaDestino = tablero.obtenerPieza(destino);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            // Prohibimos capturar reyes
                            //Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
                            //if (reyDestino) {
                            //    continue;
                            //}
                            
                            //Prohibimos capturar reyes: SIN USAR DYNAMIC CAST
                            if (piezaDestino && piezaDestino->getTipo() == t_Pieza::REY)
                                continue;

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
        return false; // Si está en jaque, no es ahogado
    }

    // Buscar si existe al menos un movimiento legal
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == color) {
                for (int f2 = 0; f2 < MAXF; ++f2) {
                    for (int c2 = 0; c2 < MAXC; ++c2) {
                        Posicion destino(f2, c2);
                        Pieza* piezaDestino = tablero.obtenerPieza(destino);

                        // Prohibimos capturar reyes
                        //Rey* reyDestino = dynamic_cast<Rey*>(piezaDestino);
                        //if (reyDestino) continue;
                        //SIN USAR DYNAMIC CAST
                        if (piezaDestino && piezaDestino->getTipo() == t_Pieza::REY)
                            continue;

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
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Posicion pos(fila, col);
            Pieza* pieza = tablero.obtenerPieza(pos);
            if (pieza) {
                /*
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
                */
                //SIN DYNAMIC CAST

                if (pieza->getTipo() == t_Pieza::REY) {
                    reyes++;
                }
                else if (pieza->getTipo() == t_Pieza::ALFIL) {
                    alfiles++;
                }
                else if (pieza->getTipo() == t_Pieza::CABALLO) {
                    caballos++;
                }
                else if (pieza->getTipo() == t_Pieza::TORRE) {
                    torres++;
                }
                else if (pieza->getTipo() == t_Pieza::REINA) {
                    reinas++;
                }
                else if (pieza->getTipo() == t_Pieza::PEON) {
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

string Juego::serializarTablero() const {
    string estado;
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Pieza* pieza = tablero.obtenerPieza(Posicion(fila, col));
            if (pieza) {
                estado += pieza->getSimbolo();
            }
            else {
                estado += '.';
            }
        }
    }
    estado += (obtenerTurnoActual() == Color::BLANCO ? 'B' : 'N'); // Añadimos turno actual para diferenciar
    return estado;
}

void Juego::registrarEstadoTablero() {
    string estado = serializarTablero();
    historialTableros[estado]++;
}

bool Juego::hayTripleRepeticion() const {
    //for (const auto& par : historialTableros) {
    //    if (par.second >= 3) {
    //       return true;
    //    }
    //}
    // 
    for (auto it = historialTableros.begin(); it != historialTableros.end(); ++it) {
        if (it->second >= 3) {
            return true;
        }
    }
    return false;
}

bool Juego::jugarTurnoBotNoob() {
    srand(time(nullptr)); // Inicializamos el random
    vector<pair<Posicion, Posicion>> movimientosValidos;

    // Recorremos el tablero en busca de piezas del bot
    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == turnoActual) {
                // Probar todos los posibles destinos
                for (int f = 0; f < MAXF; ++f) {
                    for (int c = 0; c < MAXC; ++c) {
                        Posicion destino(f, c);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            //Verificamos que no se intente capturar un rey
                            Pieza* piezaDestino = tablero.obtenerPieza(destino);
                            //if (piezaDestino && dynamic_cast<Rey*>(piezaDestino)) {
                            //    continue; // no permitimos capturar al rey
                            //}
                            //SIN USAR DYNAMIC CAST
                            if (piezaDestino && piezaDestino->getTipo() == t_Pieza::REY)
                                continue;

                            // Simulamos el movimiento
                            Pieza* capturada = tablero.obtenerPieza(destino);
                            tablero.moverPiezaSimulacion(origen, destino); // Come pieza
                            bool enJaque = estaEnJaque(turnoActual); // Comprobamos si esta en jaque despues de hacer el movimiento
                            tablero.moverPiezaSimulacion(destino, origen); // Vuelve a la posicion anterior
                            tablero.colocarPieza(capturada, destino);

                            if (!enJaque) {
                                // Guardamos cualquier movimiento que no ponga en riesgo al rey                            
                                movimientosValidos.push_back({ origen, destino });
                            }
                        }
                    }
                }
            }
        }
    }

    if (movimientosValidos.empty()) {
        cout << "El bot no tiene movimientos legales disponibles.\n";
        return false;
    }

    // Elegimos uno al azar
    int idx = rand() % movimientosValidos.size(); // [0, size()]
    Posicion origen = movimientosValidos[idx].first;
    Posicion destino = movimientosValidos[idx].second;

    Pieza* pieza = tablero.obtenerPieza(origen);
    Pieza* piezaCapturada = tablero.obtenerPieza(destino);

    tablero.moverPiezaSimulacion(origen, destino);

    //bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
    bool muevePeon = false;
    if (pieza && pieza->getTipo() == t_Pieza::PEON)
        muevePeon = true;

    bool capturaPieza = piezaCapturada != nullptr;
    if (muevePeon || capturaPieza) {
        movimientosSinCapturaNiPeon = 0;
    }
    else {
        movimientosSinCapturaNiPeon++;
    }

    // Verifica si el peón debe coronarse
    Pieza* piezaMovida = tablero.obtenerPieza(destino);
    if (piezaMovida && piezaMovida->getTipo() == t_Pieza::PEON) {
        int filaFinal = (piezaMovida->getColor() == Color::BLANCO) ? 5 : 0;
        // (condicion) ? cierto : false
        if (destino.fila == filaFinal) {
            delete piezaMovida;
            tablero.colocarPieza(new Reina(turnoActual, destino), destino); // Por defecto, Reina
        }
    }

    cambiarTurno();
    registrarEstadoTablero();
    verificarCondicionesDeTablas(vsMaquina);
    if (esJaqueMate(turnoActual)) {
        Renderizador::mensajeEstado = "Jaque mate! Ganan " + (turnoActual == Color::BLANCO ? string("negras") : string("blancas"));
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
    else if (estaAhogado(turnoActual)) {
        Renderizador::mensajeEstado = "Empate por ahogado!";
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
    else if (!tieneMaterialSuficiente()) {
        Renderizador::mensajeEstado = "Empate por material insuficiente!";
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
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

    // Bonus por coronación
    if (pieza && pieza->getTipo() == t_Pieza::PEON) {
        int filaFinal = (pieza->getColor() == Color::BLANCO) ? 5 : 0;
        if (destino.fila == filaFinal) {
            puntuacion += 9; // Valor coronar a Reina
        }
    }

    // Simula el movimiento
    tablero.moverPiezaSimulacion(origen, destino);
    if (!estaEnJaque(turnoActual)) {
        puntuacion += 10; // bonus por seguridad del rey
    }
    else {
        puntuacion -= 50; // penalización si deja al rey en jaque
    }
    tablero.moverPiezaSimulacion(destino, origen);
    tablero.colocarPieza(objetivo, destino);

    return puntuacion;
}

bool Juego::jugarTurnoBotMid() {
    srand(std::time(nullptr));
    vector<std::pair<Posicion, Posicion>> mejoresMovimientos;
    int mejorPuntaje = -100000;

    for (int fila = 0; fila < MAXF; ++fila) {
        for (int col = 0; col < MAXC; ++col) {
            Posicion origen(fila, col);
            Pieza* pieza = tablero.obtenerPieza(origen);
            if (pieza && pieza->getColor() == turnoActual) {
                for (int f = 0; f < MAXF; ++f) {
                    for (int c = 0; c < MAXC; ++c) {
                        Posicion destino(f, c);
                        if (pieza->esMovimientoValido(destino, tablero)) {
                            Pieza* objetivo = tablero.obtenerPieza(destino);
                            if (objetivo && objetivo->getTipo() == t_Pieza::REY) continue;

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
        cout << "El bot no tiene movimientos legales disponibles.\n";
        return false;
    }

    // Elegimos uno al azar
    int idx = rand() % mejoresMovimientos.size();
    Posicion origen = mejoresMovimientos[idx].first;
    Posicion destino = mejoresMovimientos[idx].second;

    Pieza* pieza = tablero.obtenerPieza(origen);
    Pieza* piezaCapturada = tablero.obtenerPieza(destino);

    tablero.moverPiezaSimulacion(origen, destino);

    //bool muevePeon = dynamic_cast<Peon*>(pieza) != nullptr;
    bool muevePeon = false;
    if (pieza && pieza->getTipo() == t_Pieza::PEON)
        muevePeon = true;

    bool capturaPieza = piezaCapturada != nullptr;
    if (muevePeon || capturaPieza) {
        movimientosSinCapturaNiPeon = 0;
    }
    else {
        movimientosSinCapturaNiPeon++;
    }

    // Verifica si el peón debe coronarse
    Pieza* piezaMovida = tablero.obtenerPieza(destino);
    if (piezaMovida && piezaMovida->getTipo() == t_Pieza::PEON) {
        int filaFinal = (piezaMovida->getColor() == Color::BLANCO) ? 5 : 0;
        if (destino.fila == filaFinal) {
            delete piezaMovida;
            tablero.colocarPieza(new Reina(turnoActual, destino), destino); // Por defecto, Reina
        }
    }

    cambiarTurno();
    registrarEstadoTablero();
    verificarCondicionesDeTablas(vsMaquina);
    if (esJaqueMate(turnoActual)) {
        Renderizador::mensajeEstado = "Jaque mate! Ganan " + (turnoActual == Color::BLANCO ? string("negras") : string("blancas"));
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
    else if (estaAhogado(turnoActual)) {
        Renderizador::mensajeEstado = "Empate por ahogado!";
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
    else if (!tieneMaterialSuficiente()) {
        Renderizador::mensajeEstado = "Empate por material insuficiente!";
        estadoActual = EstadoApp::FIN_PARTIDA;
    }
    return true;
}

Tablero& Juego::obtenerTablero() {
    return tablero;
}

void Juego::verificarCoronacion(const Posicion& destino) {
    Pieza* piezaMovida = tablero.obtenerPieza(destino);
    if (piezaMovida && piezaMovida->getTipo() == t_Pieza::PEON) {
        int filaFinal = (piezaMovida->getColor() == Color::BLANCO) ? 5 : 0;

        if (destino.fila == filaFinal) {
            // Guardamos el estado para usarlo luego desde el teclado
            enCoronacion = true;
            posicionCoronacion = destino;
            estadoActual = EstadoApp::CORONACION_PEON;
            // Esperar interacción por tecladoCallback()
            glutPostRedisplay();
        }
    }
}

void Juego::coronarPeonConTecla(char tecla) {
    if (!enCoronacion) return;

    tecla = toupper(tecla);  // Asegurar que la tecla es mayúscula
    Pieza* antigua = tablero.obtenerPieza(posicionCoronacion);
    Color colorPeon = antigua->getColor();
    //if (!antigua || !dynamic_cast<Peon*>(antigua)) return;
    if (!antigua || (antigua && antigua->getTipo() != t_Pieza::PEON)) return;

    delete antigua;

    switch (tecla) {
    case 'T':
        tablero.colocarPieza(new Torre(colorPeon, posicionCoronacion), posicionCoronacion);
        break;
    case 'A':
        tablero.colocarPieza(new Alfil(colorPeon, posicionCoronacion), posicionCoronacion);
        break;
    case 'C':
        tablero.colocarPieza(new Caballo(colorPeon, posicionCoronacion), posicionCoronacion);
        break;
    case 'R':
    default:
        tablero.colocarPieza(new Reina(colorPeon, posicionCoronacion), posicionCoronacion);
        break;

    }

    enCoronacion = false;
    estadoActual = EstadoApp::JUEGO;

    // Refrescar pantalla
    glutPostRedisplay();
}

void Juego::verificarCondicionesDeTablas(bool vsMaquina) {

    if ((getMovimientosSinCapturaNiPeon() >= MAX_MOV || hayTripleRepeticion()) &&
        (!vsMaquina || obtenerTurnoActual() == Color::BLANCO)) {

        cout << "Condicion de tablas detectada\n";

        if (getMovimientosSinCapturaNiPeon() >= MAX_MOV) {
            Renderizador::mensajeEstado = "50 movimientos sin captura ni movimiento de peon.";
        }
        else {
            Renderizador::mensajeEstado = "Triple repeticion de la posicion.";
        }

        estadoActual = EstadoApp::SOLICITUD_TABLAS;
        glutPostRedisplay();
    }
}
void Juego::verificarTiempoAgotado() {

    if ((tipoVsMaquina == false)) {
        auto ahora = std::chrono::steady_clock::now();
        int tiempoPasado = std::chrono::duration_cast<std::chrono::seconds>(ahora - inicioTurno).count();

        if (turnoActual == Color::BLANCO) {
            if (tiempoBlanco - tiempoPasado <= 0) {
                tiempoBlanco = 0;
                Renderizador::mensajeEstado = "¡Tiempo agotado! Ganan negras.";
                estadoActual = EstadoApp::FIN_PARTIDA;
            }
        }
        else {
            if (tiempoNegro - tiempoPasado <= 0) {
                tiempoNegro = 0;
                Renderizador::mensajeEstado = "¡Tiempo agotado! Ganan blancas.";
                estadoActual = EstadoApp::FIN_PARTIDA;
            }
        }

    }
}