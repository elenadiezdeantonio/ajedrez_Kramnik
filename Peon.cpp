#include "Peon.h"
#include <iostream>
using namespace std;

Peon::Peon(int _f, int _c, char _col) : Pieza(_f, _c, _col) {

}

Peon::~Peon() {

}

void Peon::print() const {
	cout << 'P';
}

void Peon::mover(int newFila, int newCol) {

}