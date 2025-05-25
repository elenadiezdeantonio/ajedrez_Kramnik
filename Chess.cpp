#include "juego.h"
#include "renderizador.h"
#include "interfaz.h"
#include "freeglut.h"

int main(int argc, char** argv) {
    // Inicializar GLUT y OpenGL
    Renderizador::inicializar(argc, argv);

    // Registrar funciones de renderizado y manejo del mouse
    InterfazUsuario::registrarCallbacks();

    // Crear instancia del juego
    static Juego juego;

    // Establecer el juego para la interfaz y el renderizador
    InterfazUsuario::establecerJuego(&juego);
    Renderizador::establecerJuego(&juego);

    // NO iniciar el juego aquí, se hará después de seleccionar el modo
    Renderizador::establecerTablero(&juego.obtenerTablero());
    // Iniciar el bucle principal de eventos
    glutMainLoop();

    return 0;
}












