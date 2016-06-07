#pragma once
#ifndef MATRIZ_H
#define MATRIZ_H

#include "Punto.h"

using namespace std;

/* Matriz de 3x3 */

class Matriz {
	private:
		Punto* filas;
	public:
		Matriz();
		Matriz(Punto* f);
		Matriz(const Matriz &m);

		void reduccionJacobi();

		~Matriz();
};

#endif
