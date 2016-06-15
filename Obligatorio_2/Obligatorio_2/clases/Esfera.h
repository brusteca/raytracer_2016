#pragma once
#ifndef ESFERA_H
#define ESFERA_H

#include <iostream>

#include "Shape.h"
#include "Punto.h"
#include "Utils.h"
#include "Mundo.h"
#include "Luz.h"

using namespace std;

class Esfera : public Shape {
	private:
		float radio;
		Punto centro;
	public:
		Esfera();
		Esfera(Punto c, float r, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp);
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);
		Punto calcularNormal(Punto p);
		~Esfera();
};

#endif
