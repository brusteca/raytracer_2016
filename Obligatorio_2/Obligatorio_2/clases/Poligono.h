#pragma once
#ifndef POLIGONO_H
#define POLIGONO_H

#include <iostream>

#include "Shape.h"
#include "Utils.h"

using namespace std;

/* Cosas para arreglar: que el �ltimo v�rtice forme arista con el primero
	y ver el caso de rayo desde afuera del pol�gono y que entre en contacto con un v�rtice*/

/* 
 * La clase pol�gono no admite figuras de menos de 3 puntos.
 * Los pol�gonos se reconocen en el sentido en que sus v�rtices son enviados,
 * por lo que los primeros 2 v�rtices generan una frontera, los v�rtices 2 y 3 generan otra, etc.
*/

class Poligono : public Shape {
	private:
		Punto* puntos;
		int cantidad;
		// Atributos para c�lculo de intersecciones
		Punto normal;
		float d; // Coeficiente de traslaci�n de plano.
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
			normal = (ps[1] + ps[0].negado()).productoVectorial((ps[2] + ps[0].negado()));			
			// 'd' se haya mediante N . P = -d, siendo P un punto del plano (se utiliza el primero).
			d = -(normal * ps[0]);			

			// Se debe chequear que todos los puntos dados pertenezcan al mismo plano, sin� el pol�gono ser� inv�lido
			// NO IMPLEMENTADO 

		};

		/* La colisi�n entre un rayo y un pol�gono se analiza en dos pasos:
				1) Colisionar rayo con plano en el que se encuentra el pol�gono.
				2) Si hay colisi�n entonces averiguar si el rayo cae dentro del pol�gono.
				
		*/
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
			if (cantidad < 3)
				return 0;
			/* 
			 * Colisionar rayo con el plano
			 * Considero ecuaci�n param�trica del rayo: p1 + t * (p2 - p1)
			 * Entonces t = -(N . p1 + d) / (N . (p2 - p1)), donde N es la normal del plano			 
			*/
			// Calculo N . (p2 - p1) para ver si hay intersecci�n
			float denominador = normal * (p2 - p1);			
			if (denominador == 0.0)
				return 0;
			// Denominador no es cero, puedo continuar
			float t = (-(normal * p1 + d)) / denominador;
			Punto interseccion = Punto (p1 + (p2 - p1).productoEscalar(t));
			// Hallada la intersecci�n con el plano, debemos ver si ese punto pertenece al pol�gono en s�.
			// Proyectar ortogonalmente todo el plano sobre la direcci�n con mayor m�dulo en su normal.
			Punto* puntosProyectados = new Punto[cantidad];
			if ((absFloat(normal.getX()) >= absFloat(normal.getY())) && (absFloat(normal.getX()) >= absFloat(normal.getZ())))
				for (int i = 0; i < cantidad; i++) {
					puntosProyectados[i] = Punto(puntos[i].getY(),puntos[i].getZ(),0.0);
				}
			else if ((absFloat(normal.getY()) >= absFloat(normal.getX())) && (absFloat(normal.getY()) >= absFloat(normal.getZ())))
				for (int i = 0; i < cantidad; i++) {
					puntosProyectados[i] = Punto(puntos[i].getX(), puntos[i].getZ(), 0.0);
				}
			else if ((absFloat(normal.getZ()) >= absFloat(normal.getX())) && (absFloat(normal.getZ()) >= absFloat(normal.getY())))
				for (int i = 0; i < cantidad; i++) {
					puntosProyectados[i] = Punto(puntos[i].getX(), puntos[i].getY(), 0.0);
				}
			// Ahora podemos trabajar en dos dimensiones, lo que simplifica ampliamente los c�lculos					
			// Lanzar un rayo desde el punto de contacto en cualquier direcci�n, en este caso direcci�n (1,0)	
			// Contar intersecciones con fronteras del pol�gono, si hay impar entonces est� dentro.
			int cantIntersecciones = 0;
			/*
			 * Rayo: interseccion + t1 * (1,0,0)
			 * Frontera entre v�rtices v1 y v2: v1 + t2 * (v2 - v1)
			 * t1 >= 0 y 0 <= t2 < 1 son las condiciones de choque exitoso.
			 */
			float t1, t2, paralelismo;
			for (int i = 0; i < cantidad - 1; i++) {
				// Los resultados fueron calculados previamente
				paralelismo = -(puntosProyectados[i + 1].getY() - puntosProyectados[i].getY());
				if (paralelismo != 0.0) {
					t2 = (puntosProyectados[i].getY() - interseccion.getY())/paralelismo;
					if ((t2 >= 0.0) && (t2 < 1.0)) {
						t1 = puntosProyectados[i].getX() - interseccion.getX() + (puntosProyectados[i + 1].getX() - puntosProyectados[i].getX()) * t2;
						if (t1 >= 0)
							cantIntersecciones++;
					}
				}
			}
			// Caso especial de �ltimo v�rtice con primero
			paralelismo = -(puntosProyectados[0].getY() - puntosProyectados[cantidad - 1].getY());
			if (paralelismo != 0.0) {
				t2 = (puntosProyectados[cantidad - 1].getY() - interseccion.getY()) / paralelismo;
				if ((t2 >= 0.0) && (t2 < 1.0)) {
					t1 = puntosProyectados[cantidad - 1].getX() - interseccion.getX() + (puntosProyectados[0].getX() - puntosProyectados[0].getX()) * t2;
					if (t1 >= 0)
						cantIntersecciones++;
				}
			}
			delete[] puntosProyectados;
			if (cantIntersecciones % 2 == 1) {
				resultado = new Punto[1];
				resultado[0] = interseccion;
				return 1;
			}
			else {
				return 0;
			}						
		};

		Punto calcularNormal(Punto p) {
			//NO IMPLEMENTADO
			return Punto();
		}
		~Poligono() {
			if (puntos)
				delete [] puntos; 
		};
};

#endif
