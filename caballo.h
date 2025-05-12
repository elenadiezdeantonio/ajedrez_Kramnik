#pragma once
#include "pieza.h"
class Caballo :public Pieza
{
public:
	Caballo(Color color, Posicion pos);
	char getSimbolo()const override;
	bool esMovimientoValido(Posicion destino, const Tablero& tablero)const override;
};