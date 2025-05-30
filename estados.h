#pragma once
enum class EstadoApp {
    MENU_PRINCIPAL,
    SELECCION_MODO,
    SELECCION_TIPO_JUEGO,
    SELECCION_DIFICULTAD,
    SELECCION_ESTILO,
    JUEGO,
    FIN_PARTIDA,
    CORONACION_PEON,
    SOLICITUD_TABLAS,
    JUGAR_DE_NUEVO
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
    MID,
    HARD
};

extern DificultadBot dificultadSeleccionada;