#pragma once
#include "tablero.h"
#include "estados.h"
#include "juego.h"
class Renderizador {
private:
    static Tablero* tablero;
    static Juego* juego;
public:
    static void inicializar(int argc, char** argv);
    static void establecerTablero(Tablero* t);
    static void dibujar();
    static void dibujarCasilla(int fila, int col);
    static void dibujarPieza(Pieza* pieza, int fila, int col);

    static void manejarMouse(int boton, int estado, int x, int y);
    static void mostrarMenu();
    static void mostrarSeleccionModo();
    static void cambiarAEstadoJuego();
    static EstadoApp obtenerEstadoActual();
    static void establecerJuego(Juego* j);
};




