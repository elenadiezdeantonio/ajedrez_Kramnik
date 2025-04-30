#include "tablero.h"
#include <iostream>
Tablero::Tablero() {
	for (int fila = 0; fila < filas; ++fila) {
		for (int col = 0; col < columnas; ++col) {
			casillas[fila][col] = nullptr;
		}
	}
}
Tablero::~Tablero() {
	for (int fila = 0; fila < filas; ++fila) {
		for (int col = 0; col < columnas; ++col) {
			delete casillas[fila][col];
		}
	}
}
void Tablero::colocarPieza(Pieza* pieza, Posicion pos) {
	casillas[pos.fila][pos.columna] = pieza;
}
Pieza* Tablero::obtenerPieza(Posicion pos)const {
	return casillas[pos.fila][pos.columna];
}
void Tablero::moverPieza(Posicion origen, Posicion destino) {
	Pieza* pieza = casillas[origen.fila][origen.columna];
	if (pieza && pieza->esMovimientoValido(destino, *this)) {
		delete casillas[destino.fila][destino.columna];
		casillas[destino.fila][destino.columna] = pieza;
		casillas[origen.fila][origen.columna] = nullptr;
		pieza->setPosicion(destino);
	}
}
void Tablero::mostrar()const {
	for (int fila = filas - 1; fila >= 0; --fila) {
		for (int col = 0; col < columnas; ++col) {
			Pieza* pieza = casillas[fila][col];
			if (pieza) {
				std::cout << pieza->getSimbolo() << ' ';
			}
			else {
				std::cout << ". ";
			}
		}
		std::cout << "\n";
	}
}