#pragma once
enum class EstadoApp {
    MENU_PRINCIPAL,
    SELECCION_MODO,
    SELECCION_TIPO_JUEGO,
    JUEGO
};

extern EstadoApp estadoActual;

enum class ModoJuego {
    MODO_5x6,
    MODO_PETTY
};

extern ModoJuego modoSeleccionado;
extern bool tipoVsMaquina;

