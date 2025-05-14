#pragma once
#include "tablero.h"
#include "color.h"

class Juego {
private:
    Tablero tablero;
    Color turnoActual;

public:
    Juego();
    void iniciar();
    bool jugarTurno(Posicion origen, Posicion destino);
    void mostrarTablero() const;
    Color obtenerTurnoActual() const;
    void cambiarTurno();
};


