#pragma once
#include "tablero.h"
#include "color.h"
#include <map>
#include <string>

class Juego {
private:
    Tablero tablero;
    Color turnoActual;
    int movimientosSinCapturaNiPeon = 0;
    std::map<std::string, int> historialTableros;
    std::string serializarTablero() const;

public:
    Juego();
    bool jugarTurno(Posicion origen, Posicion destino);
    void mostrarTablero() const;
    Color obtenerTurnoActual() const;
    void cambiarTurno();
    bool estaEnJaque(Color color);
    bool esJaqueMate(Color color);
    bool estaAhogado(Color color);
    bool tieneMaterialSuficiente();
    int getMovimientosSinCapturaNiPeon() const { return movimientosSinCapturaNiPeon; }
    void registrarEstadoTablero();
    bool hayTripleRepeticion() const;
    void iniciar5x6();       // Modo 1: disposición 5x6 esquinas opuestas
    void iniciarPetty(); // Modo 2: disposición Petty

};


