#pragma once
#include "color.h"
#include "posicion.h"
class Tablero;
class Pieza
{
protected:
	Color color;
	Posicion posicion;
public:
	Pieza(Color color, Posicion pos);
	virtual ~Pieza() = default;
	Color getColor()const;
	Posicion getPosicion()const;
	void setPosicion(Posicion nueva);
	virtual char getSimbolo()const = 0;
	virtual bool esMovimientoValido(Posicion destino, const Tablero& tablero)const = 0;
	virtual int getValor() const = 0;
};
