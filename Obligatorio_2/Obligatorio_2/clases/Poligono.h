#pragma once
#ifndef POLIGONO_H
#define POLIGONO_H

#include <iostream>
#include "Shape.h"
#include "Utils.h"
#include "Punto.h"

using namespace std;

/* Cosas para arreglar: que el último vértice forme arista con el primero
	y ver el caso de rayo desde afuera del polígono y que entre en contacto con un vértice*/

/* 
 * La clase polígono no admite figuras de menos de 3 puntos.
 * Los polígonos se reconocen en el sentido en que sus vértices son enviados,
 * por lo que los primeros 2 vértices generan una frontera, los vértices 2 y 3 generan otra, etc.
*/

class Poligono : public Shape {
	private:
		Punto* puntos;
		int cantidad;
		// Atributos para cálculo de intersecciones
		Punto normal;
		float d; // Coeficiente de traslación de plano.
	public:
		Poligono();
		Poligono(Punto* ps, int c);

		/* La colisión entre un rayo y un polígono se analiza en dos pasos:
				1) Colisionar rayo con plano en el que se encuentra el polígono.
				2) Si hay colisión entonces averiguar si el rayo cae dentro del polígono.
				
		*/
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);

		Punto calcularNormal(Punto p);

		~Poligono();
};

#endif
