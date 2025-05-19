#pragma once
#include "pieza.h"
class Torre :public Pieza
{
public:
	Torre(Color color, Posicion pos);
	char getSimbolo()const override;
	bool esMovimientoValido(Posicion destino, const Tablero& tablero)const override;
	int getValor() const override;
};
