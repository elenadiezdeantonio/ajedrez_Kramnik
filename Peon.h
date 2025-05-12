#pragma once
#include "pieza.h"

class Peon : public Pieza {
public:
    Peon(Color color, Posicion pos);
    char getSimbolo() const override;
    bool esMovimientoValido(Posicion destino, const Tablero& tablero) const override;
};