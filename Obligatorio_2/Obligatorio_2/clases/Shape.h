#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Utils.h"
#include "Punto.h"
#include "Color.h"

using namespace std;

class Shape{
	private:
		float reflexion;
		float refraccion;
		Color color;
	public:
		Shape() { reflexion = refraccion = 0.0; };
		Shape(float refle, float refra) {
			reflexion = refle;
			refraccion = refra;
		};
		// Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual int colisionaCon(Punto p1, Punto p2, Punto* &resultado) = 0;
		virtual Color calcularColor(Punto colision, Punto p1, Punto p2) { return color; };
		~Shape() {};
};

#endif
