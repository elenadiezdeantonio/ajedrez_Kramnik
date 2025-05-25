#include "renderizador.h"
#include "freeglut.h"

Tablero* Renderizador::tablero = nullptr;

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

    for (int fila = 0; fila < 6; ++fila)
        for (int col = 0; col < 5; ++col) {
            dibujarCasilla(fila, col);
            Pieza* pieza = tablero->obtenerPieza(Posicion(fila, col));
            if (pieza)
                dibujarPieza(pieza, fila, col);
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
