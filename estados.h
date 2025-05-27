#pragma once
enum class EstadoApp {
    MENU_PRINCIPAL,
    SELECCION_MODO,
    SELECCION_TIPO_JUEGO,
    SELECCION_DIFICULTAD,
    JUEGO,
    FIN_PARTIDA,
    CORONACION_PEON,
    SOLICITUD_TABLAS
};

extern EstadoApp estadoActual;

enum class ModoJuego {
    MODO_5x6,
    MODO_PETTY
};

extern ModoJuego modoSeleccionado;
extern bool tipoVsMaquina;

enum class DificultadBot {
    NOOB,
    MID
};

extern DificultadBot dificultadSeleccionada;

