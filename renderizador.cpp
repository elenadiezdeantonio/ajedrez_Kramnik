#include "renderizador.h"
#include "freeglut.h"

Tablero* Renderizador::tablero = nullptr;
Juego* Renderizador::juego = nullptr;
EstadoApp estadoActual = EstadoApp::MENU_PRINCIPAL;
ModoJuego modoSeleccionado = ModoJuego::MODO_5x6;
bool tipoVsMaquina = false;
DificultadBot dificultadSeleccionada = DificultadBot::NOOB;

std::string Renderizador::mensajeEstado = "";

void Renderizador::establecerJuego(Juego* j) {
    juego = j;
}

void Renderizador::inicializar(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 600);
    glutCreateWindow("Ajedrez Personalizado");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 5, 0, 6);
    glClearColor(1, 1, 1, 1);
}

void Renderizador::establecerTablero(Tablero* t) {
    tablero = t;
}

void Renderizador::dibujar() {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (estadoActual) {
    case EstadoApp::MENU_PRINCIPAL:
        mostrarMenu();
        break;
    case EstadoApp::SELECCION_MODO:
        mostrarSeleccionModo();
        break;
    case EstadoApp::SELECCION_TIPO_JUEGO:
        mostrarSeleccionTipoJuego();
        break;
    case EstadoApp::SELECCION_DIFICULTAD:
        mostrarSeleccionDificultad();
        break;
    case EstadoApp::FIN_PARTIDA:
        mostrarPantallaFinPartida();
        break;

    case EstadoApp::JUEGO:
        if (tablero) {
            for (int fila = 0; fila < 6; ++fila)
                for (int col = 0; col < 5; ++col) {
                    dibujarCasilla(fila, col);
                    Pieza* pieza = tablero->obtenerPieza(Posicion(fila, col));
                    if (pieza)
                        dibujarPieza(pieza, fila, col);
                }
        }
        break;

    }

    glutSwapBuffers();
}



void Renderizador::dibujarCasilla(int fila, int col) {
    if ((fila + col) % 2 == 0)
        glColor3f(0.8f, 0.8f, 0.8f);
    else
        glColor3f(0.3f, 0.3f, 0.3f);

    glBegin(GL_POLYGON);
    glVertex2f(col, fila);
    glVertex2f(col + 1, fila);
    glVertex2f(col + 1, fila + 1);
    glVertex2f(col, fila + 1);
    glEnd();
}

void Renderizador::dibujarPieza(Pieza* pieza, int fila, int col) {
    char simbolo = pieza->getSimbolo();

    glColor3f(0.0f, 0.0f, 0.0f);
    glRasterPos2f(col + 0.4f, fila + 0.4f);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, simbolo);
}

void Renderizador::mostrarMenu() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.5f, 4.5f);
    const char* titulo = "AJEDREZ KRAMNIK";
    for (const char* c = titulo; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    glColor3f(0.2f, 0.6f, 0.9f); // Color del botón
    glBegin(GL_POLYGON);
    glVertex2f(2, 2);
    glVertex2f(3, 2);
    glVertex2f(3, 2.5);
    glVertex2f(2, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(2.25f, 2.2f);
    const char* play = "PLAY";
    for (const char* c = play; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::cambiarAEstadoJuego() {
    estadoActual = EstadoApp::JUEGO;
    glutPostRedisplay(); // Forzar redibujo
}

EstadoApp Renderizador::obtenerEstadoActual() {
    return estadoActual;
}

void Renderizador::manejarMouse(int boton, int estado, int x, int y) {
    if (boton != GLUT_LEFT_BUTTON || estado != GLUT_DOWN) return;

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    float xOpenGL = x * 5.0f / ancho;
    float yOpenGL = (alto - y) * 6.0f / alto;

    if (estadoActual == EstadoApp::MENU_PRINCIPAL) {
        // Botón PLAY
        if (xOpenGL >= 2 && xOpenGL <= 3 && yOpenGL >= 2 && yOpenGL <= 2.5) {
            estadoActual = EstadoApp::SELECCION_MODO;
            glutPostRedisplay();
        }
    }

    else if (estadoActual == EstadoApp::SELECCION_MODO) {
        // Botón Modo 5x6
        if (xOpenGL >= 1 && xOpenGL <= 4 && yOpenGL >= 3 && yOpenGL <= 3.5) {
            modoSeleccionado = ModoJuego::MODO_5x6;
            estadoActual = EstadoApp::SELECCION_TIPO_JUEGO;
            glutPostRedisplay();
        }
        // Botón Modo Petty
        else if (xOpenGL >= 1 && xOpenGL <= 4 && yOpenGL >= 2 && yOpenGL <= 2.5) {
            modoSeleccionado = ModoJuego::MODO_PETTY;
            estadoActual = EstadoApp::SELECCION_TIPO_JUEGO;
            glutPostRedisplay();
        }
    }

    else if (estadoActual == EstadoApp::SELECCION_TIPO_JUEGO) {
        if (xOpenGL >= 1 && xOpenGL <= 4) {
            if (yOpenGL >= 3 && yOpenGL <= 3.5) {
                tipoVsMaquina = false;
                estadoActual = EstadoApp::JUEGO;
                iniciarJuegoSegunModo();
                glutPostRedisplay();
            }
            else if (yOpenGL >= 2 && yOpenGL <= 2.5) {
                tipoVsMaquina = true;
                estadoActual = EstadoApp::SELECCION_DIFICULTAD;
                glutPostRedisplay();
            }
        }
    }
    else if (estadoActual == EstadoApp::SELECCION_DIFICULTAD) {
        if (xOpenGL >= 1 && xOpenGL <= 4) {
            if (yOpenGL >= 3 && yOpenGL <= 3.5) {
                dificultadSeleccionada = DificultadBot::NOOB;
            }
            else if (yOpenGL >= 2 && yOpenGL <= 2.5) {
                dificultadSeleccionada = DificultadBot::MID;
            }
            estadoActual = EstadoApp::JUEGO;
            iniciarJuegoSegunModo();
            glutPostRedisplay();
        }
    }

}


void Renderizador::mostrarSeleccionModo() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.0f, 5.0f);
    const char* texto = "Selecciona el modo de juego:";
    for (const char* c = texto; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Modo 5x6
    glColor3f(0.4f, 0.7f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 3);
    glVertex2f(4, 3);
    glVertex2f(4, 3.5);
    glVertex2f(1, 3.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 3.2f);
    const char* texto5x6 = "Modo 5x6";
    for (const char* c = texto5x6; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Modo Petty
    glColor3f(0.8f, 0.3f, 0.3f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 2);
    glVertex2f(4, 2);
    glVertex2f(4, 2.5);
    glVertex2f(1, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 2.2f);
    const char* textoPetty = "Modo Petty";
    for (const char* c = textoPetty; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::mostrarSeleccionTipoJuego() {
    glColor3f(0, 0, 0);
    glRasterPos2f(1.0f, 4.5f);
    const char* texto = "Selecciona tipo de juego:";
    for (const char* c = texto; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Persona vs Persona
    glColor3f(0.2f, 0.6f, 0.9f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 3);
    glVertex2f(4, 3);
    glVertex2f(4, 3.5);
    glVertex2f(1, 3.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.4f, 3.2f);
    const char* pvp = "Persona vs Persona";
    for (const char* c = pvp; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón Persona vs Máquina
    glColor3f(0.2f, 0.6f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(1, 2);
    glVertex2f(4, 2);
    glVertex2f(4, 2.5);
    glVertex2f(1, 2.5);
    glEnd();

    glColor3f(1, 1, 1);
    glRasterPos2f(1.5f, 2.2f);
    const char* pvc = "Persona vs Maquina";
    for (const char* c = pvc; *c; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::mostrarSeleccionDificultad() {
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(1.5f, 4.5f);
    const char* titulo = "Selecciona la dificultad del bot:";
    for (const char* c = titulo; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón NOOB
    glColor3f(0.2f, 0.6f, 0.2f); // Verde
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 3.0f);
    glVertex2f(4.0f, 3.0f);
    glVertex2f(4.0f, 3.5f);
    glVertex2f(1.0f, 3.5f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(2.2f, 3.2f);
    const char* textoNoob = "NOOB";
    for (const char* c = textoNoob; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

    // Botón MID
    glColor3f(0.6f, 0.2f, 0.2f); // Rojo oscuro
    glBegin(GL_QUADS);
    glVertex2f(1.0f, 2.0f);
    glVertex2f(4.0f, 2.0f);
    glVertex2f(4.0f, 2.5f);
    glVertex2f(1.0f, 2.5f);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(2.3f, 2.2f);
    const char* textoMid = "MID";
    for (const char* c = textoMid; *c != '\0'; ++c)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void Renderizador::iniciarJuegoSegunModo() {
    Juego* juego = Renderizador::juego;
    if (!juego) return;

    if (modoSeleccionado == ModoJuego::MODO_5x6)
        juego->iniciar5x6();
    else
        juego->iniciarPetty();

    // Si es contra máquina y el turno actual es del bot (por ejemplo, siempre negras)
    if (tipoVsMaquina && juego->obtenerTurnoActual() == Color::NEGRO) {
        if (dificultadSeleccionada == DificultadBot::NOOB)
            juego->jugarTurnoBotNoob();
        else
            juego->jugarTurnoBotMid();
    }
}

void Renderizador::mostrarPantallaFinPartida() {
    // Fondo negro
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Texto blanco
    glColor3f(1.0, 1.0, 1.0);
    glRasterPos2f(1.5f, 4.5f);
    std::string mensaje = "FIN DE LA PARTIDA";
    for (char c : mensaje)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    glRasterPos2f(1.25f, 2.2f);
    for (char c : mensajeEstado)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
}