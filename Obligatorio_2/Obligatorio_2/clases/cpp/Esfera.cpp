#include "Esfera.h"

using namespace std;

Esfera::Esfera() : Shape() { radio = 0; centro = Punto(0, 0, 0); }

Esfera::Esfera(Punto c, float r, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) :
	Shape(refle, refra, transp, amb, dif, esp, constEsp) {
	radio = r;
	centro = c;
}
int Esfera::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	Punto diferencia = p2 - p1;
	float bhaA = pow(diferencia.getX(), 2) + pow(diferencia.getY(), 2) + pow(diferencia.getZ(), 2);
	float bhaB = 2 * (diferencia.getX()*(p1.getX() - centro.getX()) + diferencia.getY()*(p1.getY() - centro.getY()) + diferencia.getZ()*(p1.getZ() - centro.getZ()));
	float bhaC = pow(p1.getX() - centro.getX(), 2) + pow(p1.getY() - centro.getY(), 2) + pow(p1.getZ() - centro.getZ(), 2) - pow(radio, 2);
	raices rcs = bhaskara(bhaA, bhaB, bhaC);
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
	return 0;
}
Punto Esfera::calcularNormal(Punto p) {
	return Punto((p.x - centro.x) / radio,
		(p.y - centro.y) / radio,
		(p.z - centro.z) / radio
		);
}
Esfera::~Esfera() {}