#pragma once
class Posicion {
public:
    int fila;
    int columna;

    Posicion(int f, int c);
    bool operator==(const Posicion& otra) const;
    bool operator!=(const Posicion& otra) const;
};

