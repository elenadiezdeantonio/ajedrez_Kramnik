#pragma once
#include "pieza.h"
class Tablero
{
private:
	static const int filas = 6;
	static const int columnas = 5;
	Pieza* casillas[filas][columnas];
public:
	Tablero();
	~Tablero();
	void colocarPieza(Pieza* pieza, Posicion pos);
	Pieza* obtenerPieza(Posicion pos) const;
	void moverPieza(Posicion origen, Posicion destino);
	void mostrar()const;
};