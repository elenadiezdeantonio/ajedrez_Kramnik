#include "reina.h"
#include <cmath>
Reina::Reina(Color color, Posicion pos) : Pieza(color, pos) {}
char Reina::getSimbolo()const {
	if (color == Color::BLANCO) {
		return 'R';
	}
	else {
		return 'r';
	}
}
bool Reina::esMovimientoValido(Posicion destino, const Tablero& tablero) const {
	int df = std::abs(destino.fila - posicion.fila);
	int dc = std::abs(destino.columna - posicion.columna);
	return destino != posicion &&
		(destino.fila == posicion.fila || destino.columna == posicion.columna ||
			df == dc);
}