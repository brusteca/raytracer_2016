#pragma once
#ifndef LUZ_H
#define LUZ_H

#include "Punto.h"
#include "Mundo.h"

class Shape;

using namespace std;

struct Intensidad {
	float r;
	float g;
	float b;
	Intensidad(float red, float green, float blue) {
		r = red;
		g = green;
		b = blue;
	}
	Intensidad() {
		r = g = b = 0.0;
	}
};

class Luz {
	private:
		Punto posicion;
		Intensidad ambiente;
		Intensidad difuso;
		Intensidad especular;
	public:
		Luz(Punto p, Intensidad amb, Intensidad dif, Intensidad esp);
		//	Calcula la direccion en la que se ubica la luz segun el punto p
		Punto calcularDireccion(Punto p);
		//	retorna false si ese punto (perteneciente a ese shape) esta en sombra para esta luz
		//	despues cuando la hagamos mejor retorna el valor de la luz despues de pasar por los
		//	objetos transparentes
		bool determinarIluminacion(Punto p, Shape *s);

		Intensidad getAmb();
		Intensidad getDif();
		Intensidad getEsp();
};

#endif
