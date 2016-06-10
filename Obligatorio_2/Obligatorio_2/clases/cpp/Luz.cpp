#include "Luz.h"
#include "Punto.h"
#include "Shape.h"

using namespace std;


Luz::Luz(Punto p, Intensidad amb, Intensidad dif, Intensidad esp) {
	posicion = p;

	ambiente = amb;
	difuso = dif;
	especular = esp;
};
//	Calcula la direccion en la que se ubica la luz segun el punto p
Punto Luz::calcularDireccion(Punto p) {
	return Punto(posicion.x - p.x,
		posicion.y - p.y,
		posicion.z - p.z
		);
}
//	retorna la intensidad de la luz para el punto p en el shape s
//	si es 0 entonces el punto esta en sombra, duh
Intensidad Luz::determinarIluminacion(Punto p, Shape *s) {
	Punto direccion = p - posicion;
	//la intensidad arranca siendo la intensidad de la luz
	Intensidad iluminacion = difuso;
	float modulo = direccion.modulo();
	for (int i = 0; i < Mundo::inst()->shapes.size(); ++i) {
		//colisiono con todos excepto con el shape que me pasaron
		if (Mundo::inst()->shapes[i] == s)
			continue;
		Punto* puntoResultado = NULL;
		int cantPuntos = Mundo::inst()->shapes[i]->colisionaCon(posicion, p, puntoResultado);
		for (int cant = 0; cant < cantPuntos; cant++) {
			// Por cada colisión, quedarse con el punto más cercano descubierto hasta ahora
			// Ver si el punto está del lado adecuado
			Punto segmento = puntoResultado[cant] - posicion;
			if (segmento.productoInterno(direccion) >= 0) {
				// Si lo está, ver si su módulo es menor al del anterior punto más cercano
				if (segmento.modulo() < modulo) {
					float transpActual = Mundo::inst()->shapes[i]->getTransparencia();
					//si el shape es opaco pongo la intensidad en 0 y termino
					if (transpActual == 0) {
						iluminacion = Intensidad();
						//tengo que hacer esto porque con un break normal no alcanza
						//podria usar return pero capaz quiero seguir haciendo cosas al final
						goto finLoop;
					}
					iluminacion.r *= transpActual;
					iluminacion.g *= transpActual;
					iluminacion.b *= transpActual;
				}
			}
		}
	}
finLoop:
	return iluminacion;
}


Intensidad Luz::getAmb() {
	return ambiente;
}
Intensidad Luz::getDif() {
	return difuso;
}
Intensidad Luz::getEsp() {
	return especular;
}
