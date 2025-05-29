#pragma once
#include "tablero.h"
#include "color.h"
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "renderizador.h"
#include "freeglut.h"
#include <chrono>
class Juego {
private:
    Tablero tablero;
    Color turnoActual;
    int movimientosSinCapturaNiPeon = 0;
    std::map<std::string, int> historialTableros;
    std::string serializarTablero() const;
    bool enCoronacion = false;
    Posicion posicionCoronacion;
    bool vsMaquina = false;
    //TEMPORIZAODRES
    int tiempoBlanco;
    int tiempoNegro;
    std::chrono::time_point<std::chrono::steady_clock> inicioTurno;
public:
    Juego();
    bool jugarTurno(Posicion origen, Posicion destino);
    bool jugarTurnoBotNoob();  // Ejecuta un turno autom�tico para el bot noob (jugador negro)
    bool jugarTurnoBotMid();  // Ejecuta un turno autom�tico para el bot mid (jugador negro)
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
    void iniciar5x6();
    void iniciarPetty();
    int evaluarMovimiento(const Posicion& origen, const Posicion& destino);
    Tablero& obtenerTablero();
    void verificarCoronacion(const Posicion& destino);
    void coronarPeonConTecla(char tecla);
    void verificarCondicionesDeTablas(bool vsMaquina);
    int obtenerTiempoBlanco() const;
    int obtenerTiempoNegro() const;
    void verificarTiempoAgotado();
};




