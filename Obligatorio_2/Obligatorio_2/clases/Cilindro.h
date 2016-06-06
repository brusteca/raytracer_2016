#pragma once
#ifndef CILINDRO_H
#define CILINDRO_H

#include "Punto.h"
#include "Shape.h"
#include "Utils.h"

using namespace std;

/* Cilindro con eje paralelo al eje y */

class Cilindro : public Shape {
	private:
		float radio;
		Punto centroBase;
		float altura;
	public:
		Cilindro();
		Cilindro(float r, Punto cB, float a);
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);

		Punto calcularNormal(Punto p);
		~Cilindro();
};

#endif
