#include "Poligono.h"
#include "Matriz.h"
#include "Punto.h"
#include <iostream>

using namespace std;

Poligono::Poligono() : Shape() { 
	normal = Punto();
	d = 0.0;
}

Poligono::Poligono(/*Punto* ps, int c*/ vector<Punto> ps, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) :
	Shape(refle, refra, transp, amb, dif, esp, constEsp) {
	if (ps.size() < 3) {
		normal = Punto();
		d = 0.0;
		return;
	}
	puntos = ps;
	// Hallar datos del plano con los primeros 3 puntos
	// Si A, B y C son los puntos, entonces normal = (B-A) x (C-A)
	normal = (ps[0] - ps[1]).productoVectorial((ps[2] - ps[1]));
	// 'd' se haya mediante N . P = -d, siendo P un punto del plano (se utiliza el primero).
	d = -(normal * ps[0]);

	// Se debe chequear que todos los puntos dados pertenezcan al mismo plano, sin� el pol�gono ser� inv�lido
	// NO IMPLEMENTADO 

}

Poligono::Poligono(const Poligono &p){
	puntos = p.puntos;
	normal = p.normal;
	d = p.d;
}

int Poligono::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	if (puntos.size() < 3)
		return 0;
	// METODO EFICIENTE PARA TRIANGULOS
	if (puntos.size() == 3){
		
		// Crear la matriz de c�lculo
		Punto* filas = new Punto[3];
		filas[0] = Punto(puntos[1].x - puntos[0].x, puntos[2].x - puntos[0].x, - ((p2 - p1).normalizar()).x);
		filas[1] = Punto(puntos[1].y - puntos[0].y, puntos[2].y - puntos[0].y, -((p2 - p1).normalizar()).y);
		filas[2] = Punto(puntos[1].z - puntos[0].z, puntos[2].z - puntos[0].z, -((p2 - p1).normalizar()).z);
		Matriz matriz (filas);
		delete[] filas;

		// Calcular coeficientes
		float* coeficientes = new float[3];
		coeficientes[0] = p1.x - puntos[0].x;
		coeficientes[1] = p1.y - puntos[0].y;
		coeficientes[2] = p1.z - puntos[0].z;

		// Resolver sistema
		float* res = NULL;
		bool terminado; //= true;
		//res = new float[3];
		//res[0] = res[1] = res[2] = 0;
		terminado = matriz.resolverSistema(coeficientes, res);
		delete[] coeficientes;
		if ((!terminado) || (res[0] < 0) || (res[1] < 0) || (res[0] + res[1] >= 1)){
			if (terminado)
				delete[] res;
			return 0;
		}
		resultado = new Punto[1];
		delete[] res;
		float denominador = normal * (p2 - p1);
		if (denominador == 0.0)
			return 0;
		// Denominador no es cero, puedo continuar
		float t = (-(normal * p1 + d)) / denominador;
		resultado[0] = p1 + (p2 - p1).productoEscalar(t);
		return 1;
		
	}
	// METODO INEFICIENTE PARA POLIGONOS GENERICOS
	/*
	* Colisionar rayo con el plano
	* Considero ecuaci�n param�trica del rayo: p1 + t * (p2 - p1)
	* Entonces t = -(N . p1 + d) / (N . (p2 - p1)), donde N es la normal del plano
	*/
	// Calculo N . (p2 - p1) para ver si hay intersecci�n
	float denominador = normal * (p2 - p1);
	if (denominador == 0.0)
		return 0;
	// Denominador no es cero, puedo continuar
	float t = (-(normal * p1 + d)) / denominador;
	Punto interseccion = Punto(p1 + (p2 - p1).productoEscalar(t));
	Punto interProyectada;
	// Hallada la intersecci�n con el plano, debemos ver si ese punto pertenece al pol�gono en s�.
	// Proyectar ortogonalmente todo el plano sobre la direcci�n con mayor m�dulo en su normal.
	vector<Punto> puntosProyectados;
	if ((absFloat(normal.getX()) >= absFloat(normal.getY())) && (absFloat(normal.getX()) >= absFloat(normal.getZ()))) {
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getY(), puntos[i].getZ(), 0.0));
		}
		interProyectada = Punto(interseccion.y,interseccion.z,0);
	}
	else if ((absFloat(normal.getY()) >= absFloat(normal.getX())) && (absFloat(normal.getY()) >= absFloat(normal.getZ()))) {
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getX(), puntos[i].getZ(), 0.0));
		}
		interProyectada = Punto(interseccion.x, interseccion.z, 0);
	}
	else if ((absFloat(normal.getZ()) >= absFloat(normal.getX())) && (absFloat(normal.getZ()) >= absFloat(normal.getY()))) {
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getX(), puntos[i].getY(), 0.0));
		}
		interProyectada = Punto(interseccion.x, interseccion.y, 0);
	}
	// Ahora podemos trabajar en dos dimensiones, lo que simplifica ampliamente los c�lculos					
	// Lanzar un rayo desde el punto de contacto en cualquier direcci�n, en este caso direcci�n (1,0)	
	// Contar intersecciones con fronteras del pol�gono, si hay impar entonces est� dentro.
	int cantIntersecciones = 0;
	/*
	* Rayo: interseccion + t1 * (1,0,0)
	* Frontera entre v�rtices v1 y v2: v1 + t2 * (v2 - v1)
	* t1 >= 0 y 0 <= t2 < 1 son las condiciones de choque exitoso.
	*/
	float t1, t2, paralelismo;
	for (int i = 0; i < puntos.size() - 1; i++) {
		// Los resultados fueron calculados previamente
		paralelismo = -(puntosProyectados[i + 1].getY() - puntosProyectados[i].getY());
		if (paralelismo != 0.0) {
			t2 = (puntosProyectados[i].getY() - interProyectada.getY()) / paralelismo;
			if ((t2 >= 0.0) && (t2 < 1.0)) {
				t1 = puntosProyectados[i].getX() - interProyectada.getX() + (puntosProyectados[i + 1].getX() - puntosProyectados[i].getX()) * t2;
				if (t1 >= 0)
					cantIntersecciones++;
			}
		}
	}
	// Caso especial de �ltimo v�rtice con primero
	paralelismo = -(puntosProyectados[0].getY() - puntosProyectados[puntos.size() - 1].getY());
	if (paralelismo != 0.0) {
		t2 = (puntosProyectados[puntos.size() - 1].getY() - interProyectada.getY()) / paralelismo;
		if ((t2 >= 0.0) && (t2 < 1.0)) {
			t1 = puntosProyectados[puntos.size() - 1].getX() - interProyectada.getX() + (puntosProyectados[0].getX() - puntosProyectados[puntos.size() - 1].getX()) * t2;
			if (t1 >= 0)
				cantIntersecciones++;
		}
	}
	puntosProyectados.clear();
	if (cantIntersecciones % 2 == 1) {
		resultado = new Punto[1];
		resultado[0] = interseccion;
		return 1;
	}
	else {
		return 0;
	}
}

bool Poligono::perteneceA(Punto p) {
	return (normal * p == -d);
}

Punto Poligono::calcularNormal(Punto p) {
	// Precondici�n: El punto pertenece al pol�gono
	// Retornar la normal del plano normalizada:
	return normal.normalizar();
}

Poligono::~Poligono() {
	puntos.clear();
}