#include "Prisma.h"
#include "Esfera.h"
#include <iostream>

using namespace std;

Prisma::Prisma() {

}

Prisma::Prisma(vector<Punto> ps, float altura, float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp):
	Shape (refle, refra, transp, amb, dif, esp, constEsp) {
	//Ver si disponemos de 4 puntos y una altura
	if ((ps.size() < 4)||(altura == 0))
		return;
	//Calcular los tri�ngulos en base a esos 4 puntos
	//Punto directriz = ((ps[0] - ps[1]).productoVectorial(ps[1] - ps[2])).normalizar();
	Punto directriz = ((ps[1] - ps[2]).productoVectorial(ps[0] - ps[1])).normalizar();
	// Cara inferior
	vector<Punto> inferior_1;
	inferior_1.push_back(ps[0]);
	inferior_1.push_back(ps[1]);
	inferior_1.push_back(ps[2]);
	poligonos.push_back(Poligono(inferior_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> inferior_2;
	inferior_2.push_back(ps[2]);
	inferior_2.push_back(ps[3]);
	inferior_2.push_back(ps[0]);
	poligonos.push_back(Poligono(inferior_2, refle, refra, transp, amb, dif, esp, constEsp));
	// Cara Superior
	vector<Punto> superior_1;
	superior_1.push_back(ps[0] + directriz.productoEscalar(altura));
	superior_1.push_back(ps[1] + directriz.productoEscalar(altura));
	superior_1.push_back(ps[2] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(superior_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> superior_2;
	superior_2.push_back(ps[2] + directriz.productoEscalar(altura));
	superior_2.push_back(ps[3] + directriz.productoEscalar(altura));
	superior_2.push_back(ps[0] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(superior_2, refle, refra, transp, amb, dif, esp, constEsp));
	// Cara lateral frontal (vertices 0 y 1)
	vector<Punto> frontal_1;
	frontal_1.push_back(ps[0]);
	frontal_1.push_back(ps[1]);
	frontal_1.push_back(ps[0] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(frontal_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> frontal_2;
	frontal_2.push_back(ps[0] + directriz.productoEscalar(altura));
	frontal_2.push_back(ps[1] + directriz.productoEscalar(altura));
	frontal_2.push_back(ps[1]);	
	poligonos.push_back(Poligono(frontal_2, refle, refra, transp, amb, dif, esp, constEsp));
	// Cara lateral trasera (vertices 2 y 3)
	vector<Punto> trasera_1;
	trasera_1.push_back(ps[2]);
	trasera_1.push_back(ps[3]);
	trasera_1.push_back(ps[3] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(trasera_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> trasera_2;
	trasera_2.push_back(ps[3] + directriz.productoEscalar(altura));
	trasera_2.push_back(ps[2] + directriz.productoEscalar(altura));
	trasera_2.push_back(ps[2]);
	poligonos.push_back(Poligono(trasera_2, refle, refra, transp, amb, dif, esp, constEsp));
	// Cara lateral derecha (vertices 1 y 2)
	vector<Punto> derecha_1;
	derecha_1.push_back(ps[1]);
	derecha_1.push_back(ps[2]);
	derecha_1.push_back(ps[2] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(derecha_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> derecha_2;
	derecha_2.push_back(ps[2] + directriz.productoEscalar(altura));
	derecha_2.push_back(ps[1] + directriz.productoEscalar(altura));
	derecha_2.push_back(ps[1]);
	poligonos.push_back(Poligono(derecha_2, refle, refra, transp, amb, dif, esp, constEsp));
	// Cara lateral izquierda (vertices 3 y 0)
	vector<Punto> izquierda_1;
	izquierda_1.push_back(ps[0]);
	izquierda_1.push_back(ps[3]);
	izquierda_1.push_back(ps[3] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(izquierda_1, refle, refra, transp, amb, dif, esp, constEsp));
	vector<Punto> izquierda_2;
	izquierda_2.push_back(ps[3] + directriz.productoEscalar(altura));
	izquierda_2.push_back(ps[0] + directriz.productoEscalar(altura));
	izquierda_2.push_back(ps[0]);
	poligonos.push_back(Poligono(izquierda_2, refle, refra, transp, amb, dif, esp, constEsp));

	// Crear una Esfera como bounding shape
	// Calcular centro de la Esfera
	Punto diagonal = (ps[2] - ps[0]).productoEscalar(0.5);
	Punto centro = ps[0] + diagonal + directriz.productoEscalar(altura/2);
	// Calcular radio
	float radio = (ps[0] - ps[2] + directriz.productoEscalar(altura)).modulo();
	boundingShape = new Esfera(centro, radio, refle, refra, transp, amb, dif, esp, constEsp);
	//boundingShape = new Esfera(Punto(0,0,0), 2, refle, refra, transp, amb, dif, esp, constEsp);
}

int Prisma::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	// Ver si colisiona con la bounding shape
	Punto* resBS;
	int cantBS;
	cantBS = boundingShape->colisionaCon(p1, p2, resBS);
	if (cantBS == 0)
		return 0;
	delete[] resBS;
	// Si colisiona con ella, entonces encontrar d�nde
	Punto* resultadoAux = new Punto[6];
	Punto* res;
	int cant = 0;
	int cantCol = 0;
	int i = 0;
	while ((cant < 6)&&(i < poligonos.size())) {
		cantCol = poligonos[i].colisionaCon(p1,p2,res);
		if (cantCol > 0){
			// Agregar al array
			resultadoAux[cant] = res[0];
			delete[]res;
			cant++;
			// Si choqu� contra par,entonces no chequeo la impar
			if (i % 2 == 0)
				i++;
		}
		i++;
	}
	if (cant == 0)
		delete[] resultadoAux;
	else
		resultado = resultadoAux;
	return cant;
}
Punto Prisma::calcularNormal(Punto p) {	
	// Chequear a qu� pol�gono pertenece ese punto
	Punto* res = NULL;
	int i = 0;
	while ((i < poligonos.size()) && (!poligonos[i].perteneceA(p))) {
		i++;
	}
	if (i < poligonos.size())
		return poligonos[i].calcularNormal(p);
	else
		return Punto();
}

Prisma::~Prisma() { poligonos.clear();}