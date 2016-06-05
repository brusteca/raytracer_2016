#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Utils.h"
#include "Punto.h"
#include "Color.h"

using namespace std;

class Shape{
	protected:
		float reflexion;
		float refraccion;
		Color colorAmbiente;
		Color colorDifuso;
		Color colorEspecular;
	public:
		Shape() { 
			reflexion = refraccion = 0.0; 
			colorAmbiente.red = 255;
			colorAmbiente.green = 0;
			colorAmbiente.blue = 0; };
		Shape(float refle, float refra) : Shape() {
			reflexion = refle;
			refraccion = refra;
		}
		//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual int colisionaCon(Punto p1, Punto p2, Punto* &resultado) = 0;
		//	Determina el color en el punto 'colision' para el rayo ->p1p2
		virtual Color calcularColor(Punto colision, Punto p1, Punto p2) { return colorAmbiente; };
		//	Determina la normal en el punto p
		virtual Punto calcularNormal(Punto p) = 0;
		~Shape() {};
};

#endif
