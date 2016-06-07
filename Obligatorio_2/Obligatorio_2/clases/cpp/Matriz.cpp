#include "Matriz.h"

using namespace std;

Matriz::Matriz() {
	// Crea la matriz identidad
	filas = new float*[3];
	for (int i = 0; i < 3; i++) {
		filas[i] = new float[3];
		for (int j = 0; j < 3; j++) {
			if (j == i)
				filas[i][j] = 1;
			else
				filas[i][j] = 0;
		}
	}
}

Matriz::Matriz(Punto* f) {
	filas = new float*[3];
	for (int i = 0; i < 3; i++) {
		filas[i] = new float[3];
		filas[i][0] = f[i].x;
		filas[i][1] = f[i].y;
		filas[i][2] = f[i].z;
	}
}

Matriz::Matriz(const Matriz &m) {
	filas = new float*[3];
	for (int i = 0; i < 3; i++) {
		filas[i] = new float[3];
		for (int j = 0; j < 3; j++) {
			filas[i][j] = m.filas[i][j];
		}
	}
}

Punto* Matriz::resolverSistema() {
	// Escalerizar la matriz 
	// Recorrer columnas y ejecutar escalerización
	for (int j = 0; j < 3; j++){
	
	}

}

Matriz::~Matriz() {
	for (int i = 0; i < 3; i++)
		delete[] filas[i];
	delete[] filas;
}