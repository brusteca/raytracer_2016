#pragma once
#ifndef MATRIZ_H
#define MATRIZ_H

#include "Punto.h"
#include <iostream>
#include <string>

using namespace std;

/* Matriz de 3x3 */

class Matriz {
	private:
		float** filas;
	public:
		Matriz();
		Matriz(Punto* f);
		Matriz(const Matriz &m);

		bool resolverSistema(float* coeficientes, float* &retorno);

		string show();
		~Matriz();
};

#endif
