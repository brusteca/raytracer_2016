#pragma once
#ifndef PUNTO_H
#define PUNTO_H

using namespace std;

class Punto{
	private:
		float x;
		float y;
		float z;
	public:
		Punto() { x = y = z = 0; };
		Punto(float a, float b, float c) {
			x = a; y = b; z = c;
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

		// Getters
		float getX() { return x; };
		float getY() { return y; };
		float getZ() { return z; };

		// Operadores
		Punto operator= (Punto p) {
			x = p.x;
			y = p.y;
			z = p.z;
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

};

#endif
