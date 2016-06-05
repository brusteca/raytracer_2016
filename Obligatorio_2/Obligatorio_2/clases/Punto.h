#pragma once
#ifndef PUNTO_H
#define PUNTO_H
#include <math.h>
using namespace std;
//los puntos son vectores, lamentablemente la clase quedo con nombre Punto
class Punto{
	public:
		// Atributos
		float x;
		float y;
		float z;
		float modulo;

		// Funciones
		Punto() { x = y = z = 0; modulo = 0; };
		Punto(float a, float b, float c) {
			x = a; y = b; z = c;
			modulo = sqrt(x*x + y*y + z*z);
		};
		float productoInterno(Punto p) {
			return x*p.x + y*p.y + z*p.z;
		};
		Punto productoVectorial(Punto p) { 
			return Punto(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x); 
		};
		Punto sumar(Punto p) {
			return Punto(x+p.x,y+p.y,z+p.z);
		};
		Punto negado() {
			return Punto(-x,-y,-z);
		};
		Punto productoEscalar(float a) {
			return Punto(x*a,y*a,z*a);
		};

		Punto normalizar() {
			return Punto(x/modulo,y/modulo,z/modulo);
		}

		}

		// Getters
		float getX() { return x; };
		float getY() { return y; };
		float getZ() { return z; };

		//Setters
		void setX(float a) { x = a; };
		void setY(float a) { y = a; };
		void setZ(float a) { z = a; };

		// Operadores
		Punto operator= (Punto p) {
			x = p.x;
			y = p.y;
			z = p.z;
			return *this;
		};
		float operator* (Punto p) {
			return productoInterno(p);
		}
		Punto operator+ (Punto p) {
			return Punto(x+p.x,y+p.y,z+p.z);
		}
		Punto operator- (Punto p) {
			return *this + p.negado();
		}
		bool operator== (Punto p) {
			return ((x == p.x) && (y == p.y) && (z == p.z));
		}		

};

#endif
