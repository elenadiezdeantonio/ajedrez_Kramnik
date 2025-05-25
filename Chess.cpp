#include "juego.h"
#include "renderizador.h"
#include "interfaz.h"
#include "freeglut.h"

int main(int argc, char** argv) {
    Juego juego;
    juego.iniciar5x6();

    Renderizador::inicializar(argc, argv);
    Renderizador::establecerTablero(&juego.obtenerTablero());

    InterfazUsuario::establecerJuego(&juego);
    InterfazUsuario::registrarCallbacks();

    glutMainLoop();
    return 0;
}













