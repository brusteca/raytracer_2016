#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#include "Utils.h"
#include "Punto.h"

using namespace std;

class Shape{
	private:
	public:
		Shape() {};
		// Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual Punto* colisionaCon(Punto p1, Punto p2) = 0;
		~Shape() {};
};

#endif
