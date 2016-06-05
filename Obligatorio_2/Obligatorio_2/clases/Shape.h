#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Color.h"
#include "Mundo.h"
class Punto;

using namespace std;

class Shape{
	protected:
		float reflexion;
		float refraccion;
		Color colorAmbiente;
		Color colorDifuso;
		Color colorEspecular;
	public:
		Shape();
		Shape(float refle, float refra);
		//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual int colisionaCon(Punto p1, Punto p2, Punto* &resultado) = 0;
		//	Determina el color en el punto 'colision' para el rayo ->p1p2
		virtual Color calcularColor(Punto colision, Punto p1, Punto p2);
		//	Determina la normal en el punto p
		virtual Punto calcularNormal(Punto p) = 0;
		~Shape() {};
};

#endif
