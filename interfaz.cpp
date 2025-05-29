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
        estado == EstadoApp::SELECCION_DIFICULTAD ||
        estado == EstadoApp::SELECCION_ESTILO) {
        Renderizador::manejarMouse(button, state, x, y);
        return;
    }

    // Si estamos en el juego normal
    // Si estamos en el juego normal
    int anchoVentana = glutGet(GLUT_WINDOW_WIDTH);
    int altoVentana = glutGet(GLUT_WINDOW_HEIGHT);

    // Convertir a coordenadas OpenGL en el sistema ortográfico (-1,6)x(-1,7)
    float xOpenGL = (x / (float)anchoVentana) * 7.0f - 1.0f;
    float yOpenGL = ((altoVentana - y) / (float)altoVentana) * 8.0f - 1.0f;

    // Tablero visible en (0,0) a (5,6)
    if (xOpenGL >= 0 && xOpenGL < 5 && yOpenGL >= 0 && yOpenGL < 6) {
        int col = static_cast<int>(xOpenGL);
        int fila = static_cast<int>(yOpenGL);


        //CAMBIO PARA VER LA CASILLA SELECCIONADA
        static Posicion origen(-1, -1);

        if (origen.fila == -1) {
            origen = Posicion(fila, col);
            Renderizador::casillaSeleccionada = origen;
        }
        else {
            bool movValido = juego->jugarTurno(origen, Posicion(fila, col));
            if (movValido) {
                origen = Posicion(-1, -1);
                Renderizador::casillaSeleccionada = Posicion(-1, -1);
                glutPostRedisplay();

                // Verificar si debe jugar la máquina
                if (tipoVsMaquina && juego->obtenerTurnoActual() == Color::NEGRO) {
                    if (dificultadSeleccionada == DificultadBot::NOOB)
                        juego->jugarTurnoBotNoob();
                    else
                        juego->jugarTurnoBotMid();

                    glutPostRedisplay();
                }
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