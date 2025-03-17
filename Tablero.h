#ifndef TABLERO_H
#define TABLERO_H

#include "Pieza.h"
#include <vector>
#include <iostream>
using namespace std;

class Tablero {
private:
	static const int DIM = 4;
	//Pieza* tablero[DIM][DIM];
	vector<vector<Pieza*>> tablero;
	int fila;
public:
	Tablero();
	~Tablero();
	void onDraw();
	void onKeyBoardDown();
	void meterPieza(Pieza* p);
	void printTablero();
};

#endif //TABLERO_H