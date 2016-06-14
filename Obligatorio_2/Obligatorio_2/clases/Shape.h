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
		//calcula el color en el punto segun las luces
		ColorInt calcularColorLuz(Punto colision, Punto p1, Punto p2);
		//calcula el color en el punto segun la reflexion
		ColorInt calcularColorReflexion(Punto colision, Punto p1, Punto p2, int profundidad);
	public:
		Shape();
		Shape(float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp);
		//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
		virtual int colisionaCon(Punto p1, Punto p2, Punto* &resultado) = 0;
		//	Determina el color en el punto 'colision' para el rayo ->p1p2
		//	profundidad determina cuantos pasos recursivos realizara. Si es 0 no hara ninguna llamada recursiva
		virtual ColorInt calcularColor(Punto colision, Punto p1, Punto p2, int profundidad);
		virtual ColorInt calcularColorRefle(Punto colision, Punto p1, Punto p2, int profundidad);
		virtual ColorInt calcularColorRefra(Punto colision, Punto p1, Punto p2, int profundidad);
		//	Determina la normal en el punto p
		virtual Punto calcularNormal(Punto p) = 0;

		//recorre todos los shapes y retorna el tamano del array resultado. si esto es mayor a 0:
		//en resultado retorna los puntos
		//en indiceMasCercano retorna el indice(en el array) del punto mas cercano
		//en shapeResultado retorna la forma a la que corresponden los puntos retornados
		//en caso de estar presente, el shape ignorar es ignorado a la hora de realizar las colisiones
		static int trace(Punto p1, Punto p2, Punto direccion, Punto* &resultado, int &indiceMasCercano, Shape* &shapeResultado, Shape* ignorar = NULL);

		float getTransparencia();

		~Shape() {};
};

#endif
