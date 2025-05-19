#pragma once
#include "pieza.h"
class Alfil :public Pieza
{
public:
	Alfil(Color color, Posicion pos);
	char getSimbolo()const override;
	bool esMovimientoValido(Posicion destino, const Tablero& tablero)const override;
	int getValor() const override;
};


