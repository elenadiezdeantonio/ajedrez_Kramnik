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
bool Tablero::moverPieza(Posicion origen, Posicion destino) {
	Pieza* pieza = casillas[origen.fila][origen.columna];
	if (pieza && pieza->esMovimientoValido(destino, *this)) {
		delete casillas[destino.fila][destino.columna];
		casillas[destino.fila][destino.columna] = pieza;
		casillas[origen.fila][origen.columna] = nullptr;
		pieza->setPosicion(destino);
		return true;
	}
	return false;
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

bool Tablero::caminoLibre(Posicion origen, Posicion destino) const {
	int filaDir, colDir;

	if (destino.fila == origen.fila) {
		filaDir = 0;
	}
	else if (destino.fila > origen.fila) {
		filaDir = 1;
	}
	else {
		filaDir = -1;
	}

	if (destino.columna == origen.columna) {
		colDir = 0;
	}
	else if (destino.columna > origen.columna) {
		colDir = 1;
	}
	else {
		colDir = -1;
	}

	int fila = origen.fila + filaDir;
	int col = origen.columna + colDir;

	while (fila != destino.fila || col != destino.columna) {
		if (casillas[fila][col] != nullptr) {
			return false; // Hay una pieza en el camino
		}
		fila += filaDir;
		col += colDir;
	}

	return true; // El camino está libre
}