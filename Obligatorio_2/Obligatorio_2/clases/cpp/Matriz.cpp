#include "Matriz.h"

using namespace std;

Matriz::Matriz() {
	// Crea la matriz identidad
	filas = new Punto[3];
	filas[0] = Punto(1,0,0);
	filas[1] = Punto(0, 1, 0);
	filas[2] = Punto(0, 0, 1);
}

Matriz::Matriz(Punto* f) {
	filas = f;
}

Matriz::Matriz(const Matriz &m) {
	filas = new Punto[3];
	filas[0] = m.filas[0];
	filas[1] = m.filas[1];
	filas[2] = m.filas[2];
}

void Matriz::reduccionJacobi() {
	// IMPLEMENTAR
}

Matriz::~Matriz() {
	delete[] filas;
}