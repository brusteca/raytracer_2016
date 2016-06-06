#include "Prisma.h"
#include "Cilindro.h"

using namespace std;

Prisma::Prisma() {

}

Prisma::Prisma(vector<Punto> ps, float altura) {
	//Ver si disponemos de 4 puntos y una altura
	if (ps.size() < 4)
		return;
	//Calcular los triángulos en base a esos 4 puntos
	Punto directriz = ((ps[0] - ps[1]).productoVectorial(ps[1] - ps[2])).normalizar();
	// Cara inferior
	vector<Punto> inferior_1;
	inferior_1.push_back(ps[0]);
	inferior_1.push_back(ps[1]);
	inferior_1.push_back(ps[2]);
	poligonos.push_back(Poligono(inferior_1));
	vector<Punto> inferior_2;
	inferior_2.push_back(ps[2]);
	inferior_2.push_back(ps[3]);
	inferior_2.push_back(ps[0]);
	poligonos.push_back(Poligono(inferior_2));
	// Cara Superior
	vector<Punto> superior_1;
	superior_1.push_back(ps[0] + directriz.productoEscalar(altura));
	superior_1.push_back(ps[1] + directriz.productoEscalar(altura));
	superior_1.push_back(ps[2] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(inferior_1));
	vector<Punto> superior_2;
	superior_2.push_back(ps[2] + directriz.productoEscalar(altura));
	superior_2.push_back(ps[3] + directriz.productoEscalar(altura));
	superior_2.push_back(ps[0] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(superior_2));
	// Cara lateral frontal (vertices 0 y 1)
	vector<Punto> frontal_1;
	frontal_1.push_back(ps[0]);
	frontal_1.push_back(ps[1]);
	frontal_1.push_back(ps[0] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(frontal_1));
	vector<Punto> frontal_2;
	frontal_2.push_back(ps[0] + directriz.productoEscalar(altura));
	frontal_2.push_back(ps[1] + directriz.productoEscalar(altura));
	frontal_2.push_back(ps[1]);	
	poligonos.push_back(Poligono(frontal_2));
	// Cara lateral trasera (vertices 2 y 3)
	vector<Punto> trasera_1;
	trasera_1.push_back(ps[2]);
	trasera_1.push_back(ps[3]);
	trasera_1.push_back(ps[2] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(trasera_1));
	vector<Punto> trasera_2;
	trasera_2.push_back(ps[2] + directriz.productoEscalar(altura));
	trasera_2.push_back(ps[3] + directriz.productoEscalar(altura));
	trasera_2.push_back(ps[3]);
	poligonos.push_back(Poligono(trasera_2));
	// Cara lateral derecha (vertices 1 y 2)
	vector<Punto> derecha_1;
	derecha_1.push_back(ps[2]);
	derecha_1.push_back(ps[1]);
	derecha_1.push_back(ps[2] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(derecha_1));
	vector<Punto> derecha_2;
	derecha_2.push_back(ps[2] + directriz.productoEscalar(altura));
	derecha_2.push_back(ps[1] + directriz.productoEscalar(altura));
	derecha_2.push_back(ps[1]);
	poligonos.push_back(Poligono(derecha_2));
	// Cara lateral izquierda (vertices 3 y 0)
	vector<Punto> izquierda_1;
	izquierda_1.push_back(ps[0]);
	izquierda_1.push_back(ps[3]);
	izquierda_1.push_back(ps[0] + directriz.productoEscalar(altura));
	poligonos.push_back(Poligono(izquierda_1));
	vector<Punto> izquierda_2;
	izquierda_2.push_back(ps[0] + directriz.productoEscalar(altura));
	izquierda_2.push_back(ps[3] + directriz.productoEscalar(altura));
	izquierda_2.push_back(ps[3]);
	poligonos.push_back(Poligono(izquierda_2));
}

int Prisma::colisionaCon(Punto p1, Punto p2, Punto* &resultado) {
	return 0;
}
Punto Prisma::calcularNormal(Punto p) {
	return Punto();
}

Prisma::~Prisma() { poligonos.clear(); }