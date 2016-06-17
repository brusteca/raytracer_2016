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
	return acos(this->productoInterno(p) / (this->modulo() * p.modulo()));
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

inline float sqr(float x) {
	return x*x;
}

Punto Punto::rotar(Punto eje, float ang) {
	// https://en.wikipedia.org/wiki/Rotation_matrix
	eje = eje.normalizar();
	float cos0 = cos(ang);
	float sin0 = sin(ang);
	float matrizRot[3][3];
	matrizRot[0][0] = cos0 + sqr(eje.x) * (1 - cos0);
	matrizRot[0][1] = eje.x * eje.y * (1 - cos0) - eje.z * sin0;
	matrizRot[0][2] = eje.x * eje.z * (1 - cos0) + eje.y * sin0;
	matrizRot[1][0] = eje.y * eje.x * (1 - cos0) + eje.z * sin0;
	matrizRot[1][1] = cos0 + sqr(eje.y) * (1 - cos0);
	matrizRot[1][2] = eje.y * eje.z * (1 - cos0) - eje.x * sin0;
	matrizRot[2][0] = eje.z * eje.x * (1 - cos0) - eje.y * sin0;
	matrizRot[2][1] = eje.z * eje.y * (1 - cos0) + eje.x * sin0;
	matrizRot[2][2] = cos0 + sqr(eje.z) * (1 - cos0);
	return Punto(	x * matrizRot[0][0] + y * matrizRot[0][1] + z * matrizRot[0][2],
					x * matrizRot[1][0] + y * matrizRot[1][1] + z * matrizRot[1][2],
					x * matrizRot[2][0] + y * matrizRot[2][1] + z * matrizRot[2][2]
					);
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