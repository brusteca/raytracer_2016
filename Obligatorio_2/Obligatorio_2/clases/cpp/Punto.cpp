#include "Punto.h"
#include <math.h>

using namespace std;

Punto::Punto() { x = y = z = 0; };
Punto::Punto(float a, float b, float c) {
	x = a; y = b; z = c;
};
Punto::Punto(const Punto &p) {
	x = p.x;
	y = p.y;
	z = p.z;
}
float Punto::productoInterno(Punto p) {
	return x*p.x + y*p.y + z*p.z;
};
//DEBERIA funcionar. Esto es, dar el angulo 'chico' y no el 'grande'
float Punto::angulo(Punto p) {
	return acos(this->productoInterno(p) / (this->modulo * p.modulo));
}
Punto Punto::productoVectorial(Punto p) {
	return Punto(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
};
Punto Punto::sumar(Punto p) {
	return Punto(x + p.x, y + p.y, z + p.z);
};
Punto Punto::negado() {
	return Punto(-x, -y, -z);
};
Punto Punto::productoEscalar(float a) {
	return Punto(x*a, y*a, z*a);
};

Punto Punto::rotar(float ang) {
	
}

float Punto::modulo() {
	return sqrt(x*x+y*y+z*z);
}

Punto Punto::normalizar() {
	return Punto(x / modulo(), y / modulo(), z / modulo());
}

// Getters
float Punto::getX() { return x; };
float Punto::getY() { return y; };
float Punto::getZ() { return z; };

//Setters
void Punto::setX(float a) { x = a; };
void Punto::setY(float a) { y = a; };
void Punto::setZ(float a) { z = a; };

// Operadores
Punto Punto::operator= (Punto p) {
	x = p.x;
	y = p.y;
	z = p.z;
	return *this;
};
float Punto::operator* (Punto p) {
	return productoInterno(p);
}
Punto Punto::operator+ (Punto p) {
	return Punto(x + p.x, y + p.y, z + p.z);
}
Punto Punto::operator- (Punto p) {
	return *this + p.negado();
}
bool Punto::operator== (Punto p) {
	return ((x == p.x) && (y == p.y) && (z == p.z));
}