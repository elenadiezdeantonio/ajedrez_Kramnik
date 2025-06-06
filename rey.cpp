#include "rey.h"
#include <cmath>
using namespace std;
Rey::Rey(Color color, Posicion pos) : Pieza(color, pos, t_Pieza::REY) {}
char Rey::getSimbolo()const {
	if (color == Color::BLANCO) {
		return 'R';
	}
	else {
		return 'r';
	}
}
bool Rey::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
	int df = abs(destino.fila - posicion.fila);
	int dc = abs(destino.columna - posicion.columna);
	return (df <= 1 && dc <= 1) && !(df == 0 && dc == 0);
}
int Rey::getValor() const
{
	return 1000; 
}


