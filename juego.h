#pragma once
#include "tablero.h"
#include "color.h"
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>//AÑADIDA PARA EL RELOJ

#include "renderizador.h"
#include "freeglut.h"
using namespace std;
class Juego {
private:
    Tablero tablero;
    Color turnoActual;
    int movimientosSinCapturaNiPeon = 0;
    map<string, int> historialTableros;
    string serializarTablero() const;
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
    bool jugarTurnoBotNoob();  // Ejecuta un turno automático para el bot noob (jugador negro)
    bool jugarTurnoBotMid();  // Ejecuta un turno automático para el bot mid (jugador negro)
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


    //ALAMCENLAS LAS PIEZASELIMINADAS
    vector<Pieza*> piezasCapturadasPorBlancas;
    vector<Pieza*> piezasCapturadasPorNegras;

    //NUEVO MODO DE JUGO CON SU NUEVO MODELO DE SELECCION DE MOVIMIENTO
    int evaluarRespuestaDelOponente(Color oponente);
    bool jugarTurnoBotHard();

    //POSIBLIDIDAD DE JUGAR MAS PARTIDAS
    void reiniciar();


};



