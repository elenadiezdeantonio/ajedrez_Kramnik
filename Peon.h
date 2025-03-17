#pragma once
#include "Pieza.h"
using namespace std;
class Peon : public Pieza {
private:


public:
	Peon(int _f, int _c, char _col);
	~Peon();
	void print() const override;
	void mover(int newFila, int newCol) override;
};