#pragma once
#include "pieza.h"
class Reina :public Pieza
{
public:
	Reina(Color color, Posicion pos);
	char getSimbolo()const override;
	bool esMovimientoValido(Posicion destino, const Tablero& tablero)const override;
};

