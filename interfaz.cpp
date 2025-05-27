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

        // Convertir coordenadas de píxeles a coordenadas ortográficas (0–5 en X, 0–6 en Y)
        float xf = (x / (float)anchoVentana) * 5.0f;
        float yf = ((altoVentana - y) / (float)altoVentana) * 6.0f;

        // Botón "Sí" (mismo que en mostrarSolicitudTablas)
        if (xf >= 1.2f && xf <= 2.2f && yf >= 3.0f && yf <= 3.6f) {
            Renderizador::mensajeEstado = "¡Tablas reclamadas!";
            estadoActual = EstadoApp::FIN_PARTIDA;
            glutPostRedisplay();
            return;
        }

        // Botón "No"
        if (xf >= 2.8f && xf <= 3.8f && yf >= 3.0f && yf <= 3.6f) {
            estadoActual = EstadoApp::JUEGO;
            glutPostRedisplay();
            return;
        }

        return;
    }

    // Menús normales
    if (estado == EstadoApp::MENU_PRINCIPAL ||
        estado == EstadoApp::SELECCION_MODO ||
        estado == EstadoApp::SELECCION_TIPO_JUEGO ||
        estado == EstadoApp::SELECCION_DIFICULTAD) {
        Renderizador::manejarMouse(button, state, x, y);
        return;
    }

    // Si estamos en el juego normal
    int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
    int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);

    int col = x / (anchoVentana / 5);
    int fila = 5 - (y / (altoVentana / 6));

    static Posicion origen(-1, -1);

    if (origen.fila == -1) {
        origen = Posicion(fila, col);
    }
    else {
        juego->jugarTurno(origen, Posicion(fila, col));
        origen = Posicion(-1, -1);
        glutPostRedisplay();

        // Turno del bot si está activado el modo vs máquina
        if (tipoVsMaquina) {
            if (juego->obtenerTurnoActual() == Color::NEGRO) {
                if (dificultadSeleccionada == DificultadBot::NOOB) {
                    juego->jugarTurnoBotNoob();
                }
                else if (dificultadSeleccionada == DificultadBot::MID) {
                    juego->jugarTurnoBotMid();
                }
                glutPostRedisplay();
            }
        }
    }
}

void InterfazUsuario::keyboardCallback(unsigned char key, int x, int y) {
    // Si estamos en pantalla de coronación
    if (estadoActual == EstadoApp::CORONACION_PEON) {
        juego->coronarPeonConTecla(key);  // Delega en la lógica de coronación del juego
        return;
    }


}

