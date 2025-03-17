#include "Pieza.h"
#include "Tablero.h"
#include <iostream>
using namespace std;

Tablero::Tablero() {
	tablero.resize(DIM, vector<Pieza*>(DIM, nullptr));
}

Tablero::~Tablero() {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			delete tablero[i][j];
		}
	}
}
void Tablero::onDraw() {

}

void Tablero::onKeyBoardDown() {

}

void Tablero::printTablero() {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			if (tablero[i][j] != nullptr) {
				tablero[i][j]->print();
			}
			else {
				cout << "*";
			}
		}
		cout << endl;
	}
}

void Tablero::meterPieza(Pieza* p) {
	int f = p->getFila();
	int c = p->getColumna();
	tablero[f][c] = p;
}