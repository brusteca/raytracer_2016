#pragma once
#ifndef PUNTO_H
#define PUNTO_H
using namespace std;
//los puntos son vectores, lamentablemente la clase quedo con nombre Punto
class Punto{
	public:
		// Atributos
		float x;
		float y;
		float z;

		// Funciones
		Punto();
		Punto(float a, float b, float c);
		Punto(const Punto &p);
		float productoInterno(Punto p);
		//	El angulo en radianes entre this y p
		float angulo(Punto p);
		Punto productoVectorial(Punto p);
		Punto sumar(Punto p);
		Punto negado();
		Punto productoEscalar(float a);
		//	Rota this un angulo ang (en radianes) respecto a un eje en el sentido counterclockwise (creo)
		Punto rotar(Punto eje, float ang);
		float modulo();

		Punto normalizar();

		// Getters
		float getX();
		float getY();
		float getZ();

		//Setters
		void setX(float a);
		void setY(float a);
		void setZ(float a);

		// Operadores
		Punto operator= (Punto p);
		float operator* (Punto p);
		Punto operator+ (Punto p);
		Punto operator- (Punto p);
		bool operator== (Punto p);

};

#endif
