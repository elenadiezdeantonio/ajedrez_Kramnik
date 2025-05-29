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
    static Posicion casillaSeleccionada;
    static std::string mensajeEstado;
    static Juego* juego;
    static void inicializar(int argc, char** argv);
    static void establecerTablero(Tablero* t);
    static void dibujar();
    static void dibujarCasilla(int fila, int col);
    static void dibujarPieza(Pieza* pieza, float fila, float col, float escala);

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

    //JUGAR DE NUEVO
    static void mostrarPantallaJugarDeNuevo();


    static void mostrarSeleccionEstilo();
    static std::string obtenerNombreArchivo(const std::string& clave);


    //

    static EstiloVisual estiloActual;



    static void setEstadoActual(EstadoApp nuevoEstado);
    static EstadoApp getEstadoActual();

    //para poner fondo a baraja
    static GLuint texturaFondo;



    static float alphaTablero;  // Transparencia de 0.0 a 1.0

    //FUNCION QUE VA ACUTUALIZANDO LA TRANSPARENCIA SEGUN PASA EL TIEMPO(DE 0 A 0.6 SEGUNDOS)
    static void actualizarAlpha(int);



    static void dibujarTemporizador();

};

