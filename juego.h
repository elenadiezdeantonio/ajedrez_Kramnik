#pragma once
#include "tablero.h"
#include "color.h"

class Juego {
private:
    Tablero tablero;
    Color turnoActual;
    int movimientosSinCapturaNiPeon = 0;
public:
    Juego();
    void iniciar();
    bool jugarTurno(Posicion origen, Posicion destino);
    void mostrarTablero() const;
    Color obtenerTurnoActual() const;
    void cambiarTurno();
    bool estaEnJaque(Color color);
    bool esJaqueMate(Color color);
    bool estaAhogado(Color color);
    bool tieneMaterialSuficiente();
    int getmovimientosSinCapturaNiPeon() const { return movimientosSinCapturaNiPeon; }
};



