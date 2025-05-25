#include "interfaz.h"
#include "renderizador.h"
#include "freeglut.h"

Juego* InterfazUsuario::juego = nullptr;

void InterfazUsuario::establecerJuego(Juego* j) {
    juego = j;
}

void InterfazUsuario::registrarCallbacks() {
    glutDisplayFunc(displayCallback);
    glutMouseFunc(mouseCallback);
}

void InterfazUsuario::displayCallback() {
    Renderizador::dibujar();
}

void InterfazUsuario::mouseCallback(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    EstadoApp estado = Renderizador::obtenerEstadoActual();

    if (estado == EstadoApp::MENU_PRINCIPAL || estado == EstadoApp::SELECCION_MODO) {
        // Delega el evento de ratón al Renderizador en ambos estados
        Renderizador::manejarMouse(button, state, x, y);
        return;
    }

    // Si estamos en el estado JUEGO, continuar con la lógica del tablero
    int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
    int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);

    int col = x / (anchoVentana / 5);
    int fila = 5 - (y / (altoVentana / 6)); // Invertir eje Y

    static Posicion origen(-1, -1);

    if (origen.fila == -1) {
        origen = Posicion(fila, col);
    }
    else {
        juego->jugarTurno(origen, Posicion(fila, col));
        origen = Posicion(-1, -1);
        glutPostRedisplay();
    }
}
