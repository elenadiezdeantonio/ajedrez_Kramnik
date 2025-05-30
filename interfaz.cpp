#include "interfaz.h"
#include "renderizador.h"
#include "freeglut.h"
#include <windows.h>

Juego* InterfazUsuario::juego = nullptr;

void InterfazUsuario::establecerJuego(Juego* j) {
    juego = j;
}

void InterfazUsuario::registrarCallbacks() {
    glutDisplayFunc(displayCallback);
    glutMouseFunc(mouseCallback);
    glutKeyboardFunc(keyboardCallback);
}

void InterfazUsuario::displayCallback() {
    Renderizador::dibujar();
}

void InterfazUsuario::mouseCallback(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    EstadoApp estado = Renderizador::obtenerEstadoActual();

    // Si estamos en la pantalla de solicitud de tablas
    if (estado == EstadoApp::SOLICITUD_TABLAS) {
        int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
        int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);

        float xf = (x / (float)anchoVentana) * 5.0f;
        float yf = ((altoVentana - y) / (float)altoVentana) * 6.0f;

        if (xf >= 1.2f && xf <= 2.2f && yf >= 3.0f && yf <= 3.6f) {
            Renderizador::mensajeEstado = "¡Tablas reclamadas!";
            estadoActual = EstadoApp::FIN_PARTIDA;
            glutPostRedisplay();
            return;
        }

        if (xf >= 2.8f && xf <= 3.8f && yf >= 3.0f && yf <= 3.6f) {
            estadoActual = EstadoApp::JUEGO;
            glutPostRedisplay();
            return;
        }

        return;
    }
    if (estado == EstadoApp::JUGAR_DE_NUEVO) {
        int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
        int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);
        float xf = (x / (float)anchoVentana) * 5.0f;
        float yf = ((altoVentana - y) / (float)altoVentana) * 6.0f;

        if (xf >= 1.2f && xf <= 2.2f && yf >= 3.0f && yf <= 3.6f) {


            juego->reiniciar();
            estadoActual = EstadoApp::SELECCION_ESTILO;
            glutPostRedisplay();


            return;
        }

        if (xf >= 2.8f && xf <= 3.8f && yf >= 3.0f && yf <= 3.6f) {
            exit(0);
        }

        return;
    }
    // Menús normales
    if (estado == EstadoApp::MENU_PRINCIPAL ||
        estado == EstadoApp::SELECCION_MODO ||
        estado == EstadoApp::SELECCION_TIPO_JUEGO ||
        estado == EstadoApp::SELECCION_DIFICULTAD ||
        estado == EstadoApp::SELECCION_ESTILO ||
        estado == EstadoApp::JUGAR_DE_NUEVO) {
        Renderizador::manejarMouse(button, state, x, y);
        return;
    }

    // Si estamos en el juego normal
    int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
    int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);

    float xOpenGL = (x / (float)anchoVentana) * 7.0f - 1.0f;
    float yOpenGL = ((altoVentana - y) / (float)altoVentana) * 8.0f - 1.0f;

    if (xOpenGL >= 0 && xOpenGL < 5 && yOpenGL >= 0 && yOpenGL < 6) {
        int col = static_cast<int>(xOpenGL);
        int fila = static_cast<int>(yOpenGL);

        static Posicion origen(-1, -1);

        //DESELECIONAR CASILLA MARCADA
        if (origen.fila != -1) {

            // METODO 1 DAR CLIC EN LA MISMA CASILLA
            if (origen.fila == fila && origen.columna == col) {
                origen = Posicion(-1, -1);
                Renderizador::casillaSeleccionada = Posicion(-1, -1);
                glutPostRedisplay();
                return;
            }

            // Intentar mover
            bool movValido = juego->jugarTurno(origen, Posicion(fila, col));
            if (movValido) {
                origen = Posicion(-1, -1);
                Renderizador::casillaSeleccionada = Posicion(-1, -1);
                glutPostRedisplay();

                if (tipoVsMaquina && juego->obtenerTurnoActual() == Color::NEGRO) {
                    if (dificultadSeleccionada == DificultadBot::NOOB)
                        juego->jugarTurnoBotNoob();
                    else
                        juego->jugarTurnoBotMid();

                    glutPostRedisplay();
                }
            }
            else {
                // METODO 2 MOVER A UNA POSICION INCORRECTA/IMPSIBLE
                origen = Posicion(-1, -1);
                Renderizador::casillaSeleccionada = Posicion(-1, -1);
                glutPostRedisplay();
            }
        }
        else {
            // Seleccionar casilla si hay una pieza del color del turno actual
            Pieza* pieza = juego->obtenerTablero().obtenerPieza(Posicion(fila, col));
            if (pieza && pieza->getColor() == juego->obtenerTurnoActual()) {
                origen = Posicion(fila, col);
                Renderizador::casillaSeleccionada = origen;
                glutPostRedisplay();
            }
        }
    }
    else {
        // METODO 3 HACER CLIC FUERA DEL TABLERO
        Renderizador::casillaSeleccionada = Posicion(-1, -1);
        glutPostRedisplay();
    }

}


void InterfazUsuario::keyboardCallback(unsigned char key, int x, int y) {
    // Si estamos en pantalla de coronación
    if (estadoActual == EstadoApp::CORONACION_PEON) {
        juego->coronarPeonConTecla(key);  
        return;
    }


}




