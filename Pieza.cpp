#include "Pieza.h"


Pieza::Pieza() {

}

Pieza::Pieza(int _fila, int _columna, char _color) : fila(_fila), columna(_columna), color(_color) {

}


Pieza::~Pieza() {

}

int Pieza::getFila() {
	return fila;
}
int Pieza::getColumna() {
	return columna;
}