#pragma once
#ifndef PLANO_H
#define PLANO_H

#include "Shape.h"
#include "Punto.h"

using namespace std;

class Plano : public Shape{
	private:
		Punto puntos[3];
		Punto normal;
		float d;
	public:
		Plano();
		Plano(Punto* ps, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp);

		int colisionaCon(Punto p1, Punto p2, Punto* resultado);
		Punto calcularNormal(Punto p);

		~Plano();

};

#endif

