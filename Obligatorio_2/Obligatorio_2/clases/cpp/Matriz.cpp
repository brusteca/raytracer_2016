#include "Matriz.h"
#include <iostream>

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

bool Matriz::resolverSistema(float* coeficientes, float* &retorno) {
	// Escalerizar la matriz 
	// Recorrer columnas y ejecutar escalerización
	for (int j = 0; j < 3; j++){
		// Si estamos en i == j entonces ver si es cero
		if (filas[j][j] == 0) {
			// Si lo es, debemos hacer swap de fila con alguna que no sea cero
			int k = j + 1;
			while ((k < 3) && (filas[k][j] == 0))
				k++;
			// Si todas por debajo son cero, estamos en problemas.
			if (k == 3)
				return false;
			// Sino, hacemos swap de filas
			float* aux; //= new float[3];
			//aux[0] = filas[j][0];
			//aux[1] = filas[j][1];
			//aux[2] = filas[j][2];
			aux = filas[j];
			//filas[j][0] = filas[k][0];
			//filas[j][1] = filas[k][1];
			//filas[j][2] = filas[k][2];
			filas[j] = filas[k];
			//filas[k][0] = aux[0];
			//filas[k][1] = aux[1];
			//filas[k][2] = aux[2];
			filas[k] = aux;
			//delete[] aux;
			// Hay que swappear en coeficientes también
			float auxFloat = coeficientes[j];
			coeficientes[j] = coeficientes[k];
			coeficientes[k] = auxFloat;
		}
		// Recorrer filas inferiores a la altura de la diagonal para escalerizar
		for (int i = j + 1; i < 3; i++) {
			// Para toda fila cuyo elemento de esta columna no sea cero, modificar para que sea cero
			if (filas[i][j] != 0) {
				// Actualizar Coeficientes
				coeficientes[i] = (coeficientes[i] * filas[j][j]) - (coeficientes[j] * filas[i][j]);
				for (int k = 3 - 1; k >= j; k--) {
					filas[i][k] = (filas[i][k] * filas[j][j]) - (filas[j][k] * filas[i][j]);
				}
			}
		}
	}
	// Matriz escalerizada, resta retornar el vector solución
	// Debe calcularse el retorno en base a los coeficientes
	retorno = new float[3];
	for (int i = 3 -1 ; i >= 0; i--) {
		retorno[i] = coeficientes[i];
		for (int j = 3 - 1; j > i; j--) {
			retorno[i] -= retorno[j] * filas[i][j];
		}
		retorno[i] = retorno[i] / filas[i][i];
	}
	return true;
}

string Matriz::show() {
	string retorno = "";
	for (int i = 0; i < 3; i++) {
		retorno = retorno + "| ";
		for (int j = 0; j < 3; j++) {
			retorno = retorno + to_string(filas[i][j]) + " ";
		}
		retorno = retorno + " |\n";
	}
	return retorno;
}

Matriz::~Matriz() {
	for (int i = 0; i < 3; i++)
		delete[] filas[i];
	delete[] filas;
}