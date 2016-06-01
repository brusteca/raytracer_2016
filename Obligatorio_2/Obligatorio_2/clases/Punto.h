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
		float getX() { return x; };
		float getY() { return y; };
		float getZ() { return z; };
		Punto operator= (Punto p) {
			x = p.x;
			y = p.y;
			z = p.z;
		};
};

#endif
