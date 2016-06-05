#pragma once
#ifndef LUZ_H
#define LUZ_H

#include "Punto.h"
#include "Mundo.h"

class Shape;

using namespace std;

class Luz {
	private:
		Punto posicion;
		float intensidad;
	public:
		Luz();
		Luz(Punto p, float i);
		//	Calcula la direccion en la que se ubica la luz segun el punto p
		Punto calcularDireccion(Punto p);
		//	retorna false si ese punto (perteneciente a ese shape) esta en sombra para esta luz
		//	despues cuando la hagamos mejor retorna el valor de la luz despues de pasar por los
		//	objetos transparentes
		bool determinarIluminacion(Punto p, Shape *s);
};

#endif
