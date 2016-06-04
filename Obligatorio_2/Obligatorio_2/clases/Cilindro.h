#pragma once
#ifndef CILINDRO_H
#define CILINDRO_H

#include "Punto.h"
#include "Shape.h"
#include "Utils.h"

using namespace std;

class Cilindro : public Shape {
	private:
		float radio;
		Punto centroBase;
		float altura;
	public:
		Cilindro() : Shape() { radio = altura = 0.0; centroBase = Punto(0, 0, 0); };
		Cilindro(float r, Punto cB, float a) : Shape() {
			radio = r;
			altura = a;
			centroBase = cB;
		};
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
			
		};

		Punto calcularNormal(Punto p) {
			//NO IMPLEMENTADO
			return Punto();
		}
		~Cilindro() {};
};

#endif
