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
 * La clase polígono no admite figuras de menos de 3 puntos.
 * Los polígonos se reconocen en el sentido en que sus vértices son enviados,
 * por lo que los primeros 2 vértices generan una frontera, los vértices 2 y 3 generan otra, etc.
*/

class Poligono : public Shape {
	private:
		/*Punto* puntos;
		int cantidad;*/
		vector<Punto> puntos;
		// Atributos para cálculo de intersecciones
		Punto normal;
		float d; // Coeficiente de traslación de plano.
	public:
		Poligono();
		Poligono(/*Punto* ps, int c*/ vector<Punto> p);

		/* La colisión entre un rayo y un polígono se analiza en dos pasos:
				1) Colisionar rayo con plano en el que se encuentra el polígono.
				2) Si hay colisión entonces averiguar si el rayo cae dentro del polígono.
				
		*/
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);

		Punto calcularNormal(Punto p);

		~Poligono();
};

#endif
