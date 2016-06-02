#include <fstream>
#include <iostream>
#include <string>
#include "..\clases\Punto.h"
#include "..\clases\Poligono.h"
#include "..\clases\Shape.h"
#include "..\clases\Esfera.h"
#include "..\clases\Utils.h"
#include "..\clases\Cilindro.h"

using namespace std;


int main() {
	// Guardar la salida en un archivo
	ofstream salida;
	salida.open("testing/test01.sal");
	
	// Testear Puntos
	Punto p(1,2,3);
	Punto p2(5,5,5);
	if (!(p + p2 == Punto(6,7,8))) {
		cout << "Error: en suma" << endl ;
	}
	if (!(p - p2 == Punto(-4,-3,-2))) {
		cout << "Error en resta" << endl;
	}

	// Testear intersección
	Punto* vertices = new Punto[3];
	vertices[0] = Punto(0,0,0);
	vertices[1] = Punto(1, 1, 0);
	vertices[2] = Punto(0, 3, 0);
	Shape* pol = new Poligono(vertices, 3);
	if (!(pol->colisionaCon(Punto(0, 0, -10), Punto(1,1,0))))
		cout << "ERROR EN POLIGONO" << endl;

	salida.close();
	return 0;
}