#include "Cilindro.h"
#include "Punto.h"
#include "Utils.h"

using namespace std;

Cilindro::Cilindro() : Shape() { 
	radio = altura = 0.0;
	centroBase = Punto(0, 0, 0);
}

Cilindro::Cilindro(float r, Punto cB, float a) : Shape() {
	radio = r;
	altura = a;
	centroBase = cB;
}

/* Ecuación del tronco del cilindro con eje paralelo al eje y: x^2 + z^2 - r^2 = 0*/

int Cilindro::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	// Se debe calcular la solucion a intersectar la ecuación del tronco con la recta
	Punto u = p2 - p1;
	float bhaA = pow(u.x,2) + pow(u.z,2);
	float bhaB = 2*(u.x * p1.x + u.z * p1.z) - 2 * centroBase.x * u.x - 2*centroBase.z * u.z;
	float bhaC = pow(p1.x,2) + pow(p1.z,2) - pow(radio,2) - 2* centroBase.x*p1.x - 2*centroBase.z*p1.z + pow(centroBase.x,2) + pow(centroBase.z,2);
	raices rcs = bhaskara(bhaA, bhaB, bhaC);
	// Cálculo pensado para cámaras que no pueden rotar su dirección
	switch (rcs.cantidad) {
		case Cero:
			return 0;
			break;
		case Una: {
			Punto res = p1 + u.productoEscalar(rcs.a);
			if ((res.y >= centroBase.y) && (res.y <= centroBase.y + altura)) {
				resultado = new Punto[1];
				resultado[0] = res;
				return 1;
			}
			else {
				return 0;
			}		
			break;
		}
		case Dos: {
			Punto res1 = p1 + u.productoEscalar(rcs.a);
			Punto res2 = p1 + u.productoEscalar(rcs.b);
			if ((res1.y >= centroBase.y) && (res1.y <= centroBase.y + altura)) {
				if ((res2.y >= centroBase.y) && (res2.y <= centroBase.y + altura)) {
					resultado = new Punto[2];
					resultado[0] = res1;
					resultado[1] = res2;
					return 2;
				}
				else {
					resultado = new Punto[2];
					resultado[0] = res1;
					// Ver tapas
				}
			}
			else {
				if ((res2.y >= centroBase.y) && (res2.y <= centroBase.y + altura)) {
					resultado = new Punto[2];
					resultado[0] = res2;
					// Ver tapas
				}
				else {
					return 0;
				}
			}
			break;
		}
	}
	// Ver choque con tapas
	if (u.y == 0)
		return 1;
	// Ver punto de corte con el plano
	float t = (centroBase.y - p1.y) / u.y;
	Punto auxiliar = p1 + u.productoEscalar(t);
	// Ver si está dentro del circulo
	if (pow(auxiliar.x, 2) + pow(auxiliar.z, 2) - pow(radio, 2) < 0) {
		resultado[1] = auxiliar;
		return 2;
	}
	// Ver base superior
	t = (centroBase.y + altura - p1.y) / u.y;
	auxiliar = p1 + u.productoEscalar(t);
	if (pow(auxiliar.x, 2) + pow(auxiliar.z, 2) - pow(radio, 2) < 0) {
		resultado[1] = auxiliar;
		return 2;
	}
	return 1;
}

Punto Cilindro::calcularNormal(Punto p) {
	// Precondición: el punto pertenece al cilindro
	// Calcular según posición del punto
	float coefDePertenencia = pow(p.x, 2) + pow(p.z, 2) - pow(radio, 2);
	if (p.y == centroBase.y){
		if (coefDePertenencia < 0) {
			return Punto(0, -1, 0);
		}
		else
			return (Punto(0,-1,0) + (p - centroBase).normalizar()).normalizar();
	}
	else if (p.y == centroBase.y + altura) {
		if (coefDePertenencia < 0) {
			return Punto(0, 1, 0);
		}
		else
			return (Punto(0, 1, 0) + (p - Punto(centroBase.x,centroBase.y + altura,centroBase.z)).normalizar()).normalizar();
	}
	else {
		return (p - Punto(centroBase.x, p.y, centroBase.z)).normalizar();
	}

}

Cilindro::~Cilindro() {}