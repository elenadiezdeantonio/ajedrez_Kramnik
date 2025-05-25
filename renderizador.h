#pragma once
#include "tablero.h"

class Renderizador {
public:
    static void inicializar(int argc, char** argv);
    static void establecerTablero(Tablero* t);
    static void dibujar();
private:
    static Tablero* tablero;
    static void dibujarCasilla(int fila, int col);
    static void dibujarPieza(Pieza* pieza, int fila, int col);
};



