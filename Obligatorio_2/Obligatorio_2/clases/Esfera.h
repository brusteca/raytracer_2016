#pragma once
#ifndef ESFERA_H
#define ESFERA_H

#include <iostream>

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
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
			float bhaA = pow(p1.getX() - p2.getX(),2) + pow(p1.getY() - p2.getY(),2)  + pow(p1.getZ() - p2.getZ(),2);
			float bhaB = 2 * ((p1.getX() - p2.getX())*(p1.getX() - centro.getX()) + (p1.getY() - p2.getY())*(p1.getY() - centro.getY()) + (p1.getZ() - p2.getZ())*(p1.getZ() - centro.getZ()));
			float bhaC = pow(p1.getX() - centro.getX(),2) + pow(p1.getY() - centro.getY(), 2) + pow(p1.getZ() - centro.getZ(), 2) + pow(radio,2);
			raices rcs = bhaskara(bhaA,bhaB,bhaC);
			/* C�digo anterior que serv�a 
			float xA = p1.getX() + rcs.a * (p1.getX() - p2.getX());
			float yA = p1.getY() + rcs.a * (p1.getY() - p2.getY());
			float zA = p1.getZ() + rcs.a * (p1.getZ() - p2.getZ());
			float xB = p1.getX() + rcs.b * (p1.getX() - p2.getX());
			float yB = p1.getY() + rcs.b * (p1.getY() - p2.getY());
			float zB = p1.getZ() + rcs.b * (p1.getZ() - p2.getZ());*/
			switch (rcs.cantidad) {
				case Cero: return 0;
					break;
				case Una: 
					resultado = new Punto[1];
					resultado[0] = Punto(xA, yA, zA);
					return 1;
					break;
				case Dos: 
					resultado = new Punto[2];
					resultado[0] = p1 + (p1 - p2).productoEscalar(rcs.a);
					resultado[1] = p1 + (p1 - p2).productoEscalar(rcs.b);
					return 2;
					break;
			}
		};
		~Esfera() {};
};

#endif
