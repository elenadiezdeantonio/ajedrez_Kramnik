#include "pieza.h"
Pieza::Pieza(Color color, Posicion pos) : color(color), posicion(pos) {}
Color Pieza::getColor() const {
	return color;
}
Posicion Pieza::getPosicion()const {
	return posicion;
}
void Pieza::setPosicion(Posicion nueva) {
	posicion = nueva;
}
