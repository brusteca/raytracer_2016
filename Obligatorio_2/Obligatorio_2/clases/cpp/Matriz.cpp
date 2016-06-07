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
		// Si estamos en i == j entonces ver si es cero
		if (filas[j][j] == 0) {
			// Si lo es, debemos hacer swap de fila con alguna que no sea cero
			int k = j+1;
			while ((k < 3)&&(filas[k][j] != 0))
				k++;
			// Si todas por debajo son cero, estamos en problemas.
			if (k == 3)
				return NULL;
			// Sino, hacemos swap de filas
			float* aux = filas[j];
			filas[j] = filas[k];
			filas[k] = filas[j];
		}
		// Recorrer filas inferiores a la altura de la diagonal para escalerizar
		for (int i = j + 1; i < 3; i++) {
			// Para toda fila cuyo elemento de esta columna no sea cero, modificar para que sea cero
			if (filas[i][j] != 0) {
				float* aux = new float[3];
			}
		}
	}

}

Matriz::~Matriz() {
	for (int i = 0; i < 3; i++)
		delete[] filas[i];
	delete[] filas;
}