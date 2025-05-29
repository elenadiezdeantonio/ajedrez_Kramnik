#pragma once
#include "color.h"
#include "posicion.h"
class Tablero;

enum class t_Pieza { REY, REINA, ALFIL, CABALLO, TORRE, PEON };

class Pieza
{
protected:
	Color color;
	Posicion posicion;
	t_Pieza tipo;
public:
	Pieza(Color color, Posicion pos, t_Pieza tipo);
	virtual ~Pieza() = default;
	Color getColor()const;
	Posicion getPosicion()const;
	void setPosicion(Posicion nueva);
	virtual char getSimbolo()const = 0;
	virtual bool esMovimientoValido(Posicion destino, const Tablero& tablero)const = 0;
	virtual int getValor() const = 0;
	virtual t_Pieza getTipo() const;
};

