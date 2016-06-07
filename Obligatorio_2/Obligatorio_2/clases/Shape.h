#pragma once
#ifndef SHAPE_H
#define SHAPE_H

#define constAtt 0.03f
#define linearAtt 0.2f
#define quadAtt 0.012f
#define nPhong 5

#include "Color.h"
#include "Mundo.h"
class Punto;

using namespace std;

class Shape{
	protected:
		float reflexion;
		float refraccion;
		float transparencia;
		Color colorAmbiente;
		Color colorDifuso;
		Color colorEspecular;
		float constanteEspecular;
	public:
		Shape();
		Shape(float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp);
		//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual int colisionaCon(Punto p1, Punto p2, Punto* &resultado) = 0;
		//	Determina el color en el punto 'colision' para el rayo ->p1p2
		virtual Color calcularColor(Punto colision, Punto p1, Punto p2);
		//	Determina la normal en el punto p
		virtual Punto calcularNormal(Punto p) = 0;
		~Shape() {};
};

#endif
