#pragma once
#include "pieza.h"
class Rey :public Pieza
{
public:
	Rey(Color color, Posicion pos);
	char getSimbolo()const override;
	bool esMovimientoValido(Posicion destino, const Tablero& tablero)const override;
};

