#pragma once
#ifndef ESFERA_H
#define ESFERA_H

#include "Shape.h"
#include "Punto.h"
#include "Utils.h"

using namespace std;

class Esfera : public Shape {
	private:
		float radio;
		Punto centro;
	public:
		Esfera() { radio = 0; centro = Punto(0, 0, 0); };
		Esfera(float x, float y, float z, float r) {
			radio = r;
			centro = Punto(x,y,z);
		};
		bool colisionaCon(Punto p1, Punto p2) {
			float bhaA = pow(p1.getX() - p2.getX(),2) + pow(p1.getY() - p2.getY(),2)  + pow(p1.getZ() - p2.getZ(),2);
			float bhaB = 2 * ((p1.getX() - p2.getX())*(p1.getX() - centro.getX()) + (p1.getY() - p2.getY())*(p1.getY() - centro.getY()) + (p1.getZ() - p2.getZ())*(p1.getZ() - centro.getZ()));
			float bhaC = pow(p1.getX() - centro.getX(),2) + pow(p1.getY() - centro.getY(), 2) + pow(p1.getZ() - centro.getZ(), 2) + pow(radio,2);
			raices rcs = bhaskara(bhaA,bhaB,bhaC);
		};
		~Esfera() {};
};

#endif
