#include "Poligono.h"
#include "Punto.h"

using namespace std;

Poligono::Poligono() { 
	/*puntos = NULL;
	cantidad = 0;*/
	normal = Punto();
	d = 0.0;
}

Poligono::Poligono(/*Punto* ps, int c*/ vector<Punto> ps, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) :
	Shape(refle, refra, transp, amb, dif, esp, constEsp) {
	if (ps.size() < 3) {
		/*puntos = NULL;
		cantidad = 0;*/
		normal = Punto();
		d = 0.0;
		return;
	}
	puntos = ps;
	//cantidad = c;
	// Hallar datos del plano con los primeros 3 puntos
	// Si A, B y C son los puntos, entonces normal = (B-A) x (C-A)
	normal = (ps[1] + ps[0].negado()).productoVectorial((ps[2] + ps[0].negado()));
	// 'd' se haya mediante N . P = -d, siendo P un punto del plano (se utiliza el primero).
	d = -(normal * ps[0]);

	// Se debe chequear que todos los puntos dados pertenezcan al mismo plano, sinó el polígono será inválido
	// NO IMPLEMENTADO 

}

int Poligono::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	if (puntos.size() < 3)
		return 0;
	// METODO EFICIENTE PARA TRIANGULOS
	/*if (puntos.size() == 3)
		return 0;*/
	// METODO INEFICIENTE PARA POLIGONOS GENERICOS
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
	// Hallada la intersección con el plano, debemos ver si ese punto pertenece al polígono en sí.
	// Proyectar ortogonalmente todo el plano sobre la dirección con mayor módulo en su normal.
	vector<Punto> puntosProyectados/* = new Punto[cantidad]*/;
	if ((absFloat(normal.getX()) >= absFloat(normal.getY())) && (absFloat(normal.getX()) >= absFloat(normal.getZ())))
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getY(), puntos[i].getZ(), 0.0));
		}
	else if ((absFloat(normal.getY()) >= absFloat(normal.getX())) && (absFloat(normal.getY()) >= absFloat(normal.getZ())))
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getX(), puntos[i].getZ(), 0.0));
		}
	else if ((absFloat(normal.getZ()) >= absFloat(normal.getX())) && (absFloat(normal.getZ()) >= absFloat(normal.getY())))
		for (int i = 0; i < puntos.size()/*cantidad*/; i++) {
			puntosProyectados.push_back(Punto(puntos[i].getX(), puntos[i].getY(), 0.0));
		}
	// Ahora podemos trabajar en dos dimensiones, lo que simplifica ampliamente los cálculos					
	// Lanzar un rayo desde el punto de contacto en cualquier dirección, en este caso dirección (1,0)	
	// Contar intersecciones con fronteras del polígono, si hay impar entonces está dentro.
	int cantIntersecciones = 0;
	/*
	* Rayo: interseccion + t1 * (1,0,0)
	* Frontera entre vértices v1 y v2: v1 + t2 * (v2 - v1)
	* t1 >= 0 y 0 <= t2 < 1 son las condiciones de choque exitoso.
	*/
	float t1, t2, paralelismo;
	for (int i = 0; i < puntos.size() - 1; i++) {
		// Los resultados fueron calculados previamente
		paralelismo = -(puntosProyectados[i + 1].getY() - puntosProyectados[i].getY());
		if (paralelismo != 0.0) {
			t2 = (puntosProyectados[i].getY() - interseccion.getY()) / paralelismo;
			if ((t2 >= 0.0) && (t2 < 1.0)) {
				t1 = puntosProyectados[i].getX() - interseccion.getX() + (puntosProyectados[i + 1].getX() - puntosProyectados[i].getX()) * t2;
				if (t1 >= 0)
					cantIntersecciones++;
			}
		}
	}
	// Caso especial de último vértice con primero
	paralelismo = -(puntosProyectados[0].getY() - puntosProyectados[puntos.size() - 1].getY());
	if (paralelismo != 0.0) {
		t2 = (puntosProyectados[puntos.size() - 1].getY() - interseccion.getY()) / paralelismo;
		if ((t2 >= 0.0) && (t2 < 1.0)) {
			t1 = puntosProyectados[puntos.size() - 1].getX() - interseccion.getX() + (puntosProyectados[0].getX() - puntosProyectados[0].getX()) * t2;
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

Punto Poligono::calcularNormal(Punto p) {
	// Precondición: El punto pertenece al polígono
	// Retornar la normal del plano normalizada:
	return normal.normalizar();
}

Poligono::~Poligono() {
	puntos.clear();
}