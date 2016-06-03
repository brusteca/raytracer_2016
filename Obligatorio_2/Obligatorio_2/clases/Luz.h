#pragma once
#ifndef LUZ_H
#define LUZ_H

#include "Punto.h"

using namespace std;

class Luz {
	private:
		Punto posicion;
		float intensidad;
	public:
		Luz() { posicion = Punto(); intensidad = 0.0; };
		Luz(Punto p, float i) {
			posicion = p;
			intensidad = i;
		};
};

#endif
