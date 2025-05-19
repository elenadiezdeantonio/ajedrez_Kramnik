#include "reina.h"
#include "Tablero.h"
#include <cmath>
Reina::Reina(Color color, Posicion pos) : Pieza(color, pos) {}
char Reina::getSimbolo()const {
	if (color == Color::BLANCO) {
		return 'Q';
	}
	else {
		return 'q';
	}
}
bool Reina::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
	int df = std::abs(destino.fila - posicion.fila);
	int dc = std::abs(destino.columna - posicion.columna);
	return destino != posicion && (destino.fila == posicion.fila || destino.columna == posicion.columna || (df == dc) && tablero.caminoLibre(posicion, destino));
}
int Reina::getValor() const
{
	return 9;
}