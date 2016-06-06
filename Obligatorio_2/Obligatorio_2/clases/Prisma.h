#pragma once
#ifndef PRISMA_H
#define PRISMA_H

#include "Shape.h"
#include "Poligono.h"
#include<vector>

using namespace std;

class Prisma : public Shape {
	private:
		vector<Poligono> poligonos;
		Shape* boundingShape;
		vector<pair<Punto, Punto>> normalesDeColision;
	public:
		Prisma();
		Prisma(vector<Punto> ps, float altura);
		int colisionaCon(Punto p1, Punto p2, Punto* &resultado);
		Punto calcularNormal(Punto p);
		~Prisma();
};

#endif
