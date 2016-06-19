#include "Plano.h"

using namespace std;

Plano::Plano() {
	puntos[0] = Punto(1, 0, 0);
	puntos[1] = Punto(0, 0, 1);
	puntos[2] = Punto(1, 0, 1);
	normal = Punto(0, 1, 0);
	d = 0;
}
Plano::Plano(Punto* ps, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) : 
	Shape(refle, refra, transp, amb, dif, esp, constEsp) {
	puntos[0] = ps[0];
	puntos[1] = ps[1];
	puntos[2] = ps[2];
	// Hallar datos del plano con los primeros 3 puntos
	// Si A, B y C son los puntos, entonces normal = (B-A) x (C-A)
	normal = (ps[0] - ps[1]).productoVectorial((ps[2] - ps[1]));
	// 'd' se haya mediante N . P = -d, siendo P un punto del plano (se utiliza el primero).
	d = -(normal * ps[0]);

}

int Plano::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	/*
	* Colisionar rayo con el plano
	* Considero ecuación paramétrica del rayo: p1 + t * (p2 - p1)
	* Entonces t = -(N . p1 + d) / (N . (p2 - p1)), donde N es la normal del plano
	*/
	// Calculo N . (p2 - p1) para ver si hay intersección
	float denominador = normal * (p2 - p1);
	if (denominador == 0.0)
		return 0;
	// Denominador no es cero, puedo continuar
	float t = (-(normal * p1 + d)) / denominador;
	Punto interseccion = Punto(p1 + (p2 - p1).productoEscalar(t));
	resultado = new Punto [1];
	resultado[0] = interseccion;
	return 1;
}
Punto Plano::calcularNormal(Punto p) {
	return normal.normalizar();
}

Plano::~Plano() {

}