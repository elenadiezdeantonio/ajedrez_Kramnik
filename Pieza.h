#ifndef PIEZA_H
#define PIEZA_H
using namespace std;

class Pieza {
private:
	int fila;
	int columna;
	char color;
public:
	Pieza();
	Pieza(int _fila, int _columna, char _color);
	virtual ~Pieza();
	virtual void mover(int newFila, int newCol) = 0;
	virtual void print() const = 0;
	int getFila();
	int getColumna();
};

#endif //PIEZA_H