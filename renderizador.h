#pragma once
#include "tablero.h"
#include "estados.h"
#include "juego.h"
#include <map>
#include <string>
#include "freeglut.h"


enum class EstiloVisual {
    NORMAL,
    BARAJA,
};

class Juego;
class Renderizador {
private:
    static Tablero* tablero;
    static std::map<std::string, GLuint> texturasPiezas;
public:
    static std::string mensajeEstado;
    static Juego* juego;
    static void inicializar(int argc, char** argv);
    static void establecerTablero(Tablero* t);
    static void dibujar();
    static void dibujarCasilla(int fila, int col);
    static void dibujarPieza(Pieza* pieza, int fila, int col);

    static void manejarMouse(int boton, int estado, int x, int y);
    static void mostrarMenu();
    static void mostrarSeleccionModo();
    static void mostrarSeleccionTipoJuego();
    static void mostrarSeleccionDificultad();
    static void mostrarPantallaFinPartida();
    static void cambiarAEstadoJuego();
    static EstadoApp obtenerEstadoActual();
    static void establecerJuego(Juego* j);
    static void iniciarJuegoSegunModo();
    static void mostrarOpcionesCoronacion();
    static void mostrarSolicitudTablas(const std::string& mensajeEstado);
    static void cargarTexturasPiezas();


    static void mostrarSeleccionEstilo();
    static std::string obtenerNombreArchivo(const std::string& clave);

    static EstiloVisual estiloActual;


    static void setEstadoActual(EstadoApp nuevoEstado);
    static EstadoApp getEstadoActual();

    //para poner fondo a baraja
    static GLuint texturaFondo;
};
