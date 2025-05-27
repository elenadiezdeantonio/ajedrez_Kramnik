#pragma once
#include "juego.h"

class InterfazUsuario {
public:
    static void establecerJuego(Juego* j);
    static void registrarCallbacks();
private:
    static Juego* juego;
    static void displayCallback();
    static void mouseCallback(int button, int state, int x, int y);
    static void keyboardCallback(unsigned char key, int x, int y);
};

