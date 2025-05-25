#include "renderizador.h"
#include "freeglut.h"

Tablero* Renderizador::tablero = nullptr;
EstadoApp estadoActual = EstadoApp::MENU_PRINCIPAL;

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

    if (estadoActual == EstadoApp::MENU_PRINCIPAL) {
        mostrarMenu();
    }
    else if (estadoActual == EstadoApp::JUEGO && tablero) {
        for (int fila = 0; fila < 6; ++fila)
            for (int col = 0; col < 5; ++col) {
                dibujarCasilla(fila, col);
                Pieza* pieza = tablero->obtenerPieza(Posicion(fila, col));
                if (pieza)
                    dibujarPieza(pieza, fila, col);
            }
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
    if (boton == GLUT_LEFT_BUTTON && estado == GLUT_DOWN && estadoActual == EstadoApp::MENU_PRINCIPAL) {
        // Convertir coordenadas de pantalla a coordenadas OpenGL
        int ancho = glutGet(GLUT_WINDOW_WIDTH);
        int alto = glutGet(GLUT_WINDOW_HEIGHT);
        float xOpenGL = x * 5.0f / ancho;
        float yOpenGL = (alto - y) * 6.0f / alto;

        // Comprobar si está dentro del botón PLAY (entre (2,2) y (3,2.5))
        if (xOpenGL >= 2 && xOpenGL <= 3 && yOpenGL >= 2 && yOpenGL <= 2.5) {
            cambiarAEstadoJuego();
        }
    }
}