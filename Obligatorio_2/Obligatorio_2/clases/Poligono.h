#pragma once
#ifndef POLIGONO_H
#define POLIGONO_H

#include "Shape.h"

using namespace std;

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
		float d;
	public:
		Poligono() { puntos = NULL; cantidad = 0; normal = Punto(); d = 0.0; };
		Poligono(Punto* ps, int c) {
			if (c < 3) {
				puntos = NULL; 
				cantidad = 0;
				normal = Punto();
				d = 0.0;
				return;
			}
			puntos = ps;
			cantidad = c;
			// Hallar datos del plano con los primeros 3 puntos
			// Si A, B y C son los puntos, entonces normal = (B-A) x (C-A)
			normal = (ps[1] + ps[0].negado()) *(ps[2] + ps[0].negado());
			// 'd' se haya mediante N . P = -d, siendo P un punto del plano (se utiliza el primero).
			d = -(normal * ps[0]);

			// Se debe chequear que todos los puntos dados pertenezcan al mismo plano, sinó el polígono será inválido
			// NO IMPLEMENTADO 

		};

		/* La colisión entre un rayo y un polígono se analiza en dos pasos:
				1) Colisionar rayo con plano en el que se encuentra el polígono.
				2) Si hay colisión entonces averiguar si el rayo cae dentro del polígono.
				
		*/
		Punto* colisionaCon(Punto p1, Punto p2) {
			if (cantidad < 3)
				return NULL;
			/* 
			 * Colisionar rayo con el plano
			 * Considero ecuación paramétrica del rayo: p1 + t * (p2 - p1)
			 * Entonces t = -(N . p1 + d) / (N . (p2 - p1)), donde N es la normal del plano			 
			*/
			// Calculo N . p2 para ver si hay intersección
			float denominador = normal * (p2);
			if (denominador == 0.0)
				return NULL;
			// Denominador no es cero, puedo continuar
			float t = (-(normal * p1 + d)) / denominador;
			Punto interseccion = p1 + (p2 - p1).productoEscalar(t);
			// Hallada la intersección con el plano, debemos ver si ese punto pertenece al polígono en sí.
			// Lanzar un rayo desde el punto de contacto en cualquier dirección, en este caso dirección hacia el primer punto del polígono
			// Rayo : interseccion + t2 * (puntos[0] - interseccion)
			// Recta entre vértices v1 y v2: v1 + tv * (v2 - v1)
		};

		~Poligono() {
			if (puntos)
				delete [] puntos; 
		};
};

#endif
