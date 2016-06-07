#pragma once
#ifndef ESFERA_H
#define ESFERA_H

#include <iostream>

#include "Shape.h"
#include "Punto.h"
#include "Utils.h"
#include "Mundo.h"
#include "Luz.h"

using namespace std;

class Esfera : public Shape {
	private:
		float radio;
		Punto centro;
	public:
		Esfera() : Shape() { radio = 0; centro = Punto(0, 0, 0); };
		Esfera(Punto c, float r, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) : 
			Shape(refle, refra, transp, amb, dif, esp, constEsp) {
			radio = r;
			centro = c;
		};
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
			Punto diferencia = p2 - p1;
			float bhaA = pow(diferencia.getX(),2) + pow(diferencia.getY(),2)  + pow(diferencia.getZ(),2);
			float bhaB = 2 * (diferencia.getX()*(p1.getX() - centro.getX()) + diferencia.getY()*(p1.getY() - centro.getY()) + diferencia.getZ()*(p1.getZ() - centro.getZ()));
			float bhaC = pow(p1.getX() - centro.getX(),2) + pow(p1.getY() - centro.getY(), 2) + pow(p1.getZ() - centro.getZ(), 2) - pow(radio,2);
			raices rcs = bhaskara(bhaA,bhaB,bhaC);
			/* Código anterior que servía 
			float xA = p1.getX() + rcs.a * (p1.getX() - p2.getX());
			float yA = p1.getY() + rcs.a * (p1.getY() - p2.getY());
			float zA = p1.getZ() + rcs.a * (p1.getZ() - p2.getZ());
			float xB = p1.getX() + rcs.b * (p1.getX() - p2.getX());
			float yB = p1.getY() + rcs.b * (p1.getY() - p2.getY());
			float zB = p1.getZ() + rcs.b * (p1.getZ() - p2.getZ());*/
			switch (rcs.cantidad) {
				case Cero: 
					return 0;
					break;
				case Una: 
					resultado = new Punto[1];
					resultado[0] = p1 + diferencia.productoEscalar(rcs.a);
					return 1;
					break;
				case Dos: 
					resultado = new Punto[2];
					resultado[0] = p1 + diferencia.productoEscalar(rcs.a);
					resultado[1] = p1 + diferencia.productoEscalar(rcs.b);
					return 2;
					break;
			}
		};
		//Color calcularColor(Punto colision, Punto p1, Punto p2) {
		//	Color color;
		//	color.red = color.green = color.blue = 0;
		//	//calculo si la superficie tiene luz o sombra
		//	for (int i = 0; i < mundo.luces.size(); ++i) {
		//		Luz luz = mundo.luces[i];
		//		Punto direccionLuz = luz.calcularDireccion(colision);
		//		Punto normal = calcularNormal(colision);
		//		if ((direccionLuz * normal  > 0) &&
		//			luz.determinarIluminacion(colision, this)){

		//			color.red += colorAmbiente.red;
		//			color.green += colorAmbiente.green;
		//			color.blue += colorAmbiente.blue;
		//		}
		//	}
		//	return color;
		//}
		Punto calcularNormal(Punto p) {
			return Punto(	(p.x - centro.x) / radio,
							(p.y - centro.y) / radio,
							(p.z - centro.z) / radio
						);
		}
		~Esfera() {};
};

#endif
