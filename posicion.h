#pragma once
class Posicion {
public:
    int fila;
    int columna;

    Posicion() : fila(0), columna(0) {} 
    Posicion(int f, int c);
    bool operator==(const Posicion& otra) const;
    bool operator!=(const Posicion& otra) const;
};

