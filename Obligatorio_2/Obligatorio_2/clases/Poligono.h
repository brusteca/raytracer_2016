#pragma once
#ifndef POLIGONO_H
#define POLIGONO_H

#include <iostream>
#include <vector>
#include "Shape.h"
#include "Utils.h"
#include "Punto.h"

using namespace std;
/*
 * La clase pol�gono no admite figuras de menos de 3 puntos.
 * Los pol�gonos se reconocen en el sentido en que sus v�rtices son enviados,
 * por lo que los primeros 2 v�rtices generan una frontera, los v�rtices 2 y 3 generan otra, etc.
*/

class Poligono : public Shape {
	private:
		/*Punto* puntos;
		int cantidad;*/
		vector<Punto> puntos;
		// Atributos para c�lculo de intersecciones
		Punto normal;
		float d; // Coeficiente de traslaci�n de plano.
	public:
		Poligono();
		Poligono(/*Punto* ps, int c*/ vector<Punto> p);

		/* La colisi�n entre un rayo y un pol�gono se analiza en dos pasos:
				1) Colisionar rayo con plano en el que se encuentra el pol�gono.
				2) Si hay colisi�n entonces averiguar si el rayo cae dentro del pol�gono.
				
		*/
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);

		Punto calcularNormal(Punto p);

		~Poligono();
};

#endif
