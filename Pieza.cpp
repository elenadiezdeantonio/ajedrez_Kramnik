#include "pieza.h"
Pieza::Pieza(Color color, Posicion pos, t_Pieza tipo) : color(color), posicion(pos), tipo(tipo) {}
Color Pieza::getColor() const {
	return color;
}
Posicion Pieza::getPosicion()const {
	return posicion;
}
void Pieza::setPosicion(Posicion nueva) {
	posicion = nueva;
}

t_Pieza Pieza::getTipo()const {
	return tipo;
}