#pragma once
#ifndef LUZ_H
#define LUZ_H

#include "Punto.h"
#include "Mundo.h"

using namespace std;

class Luz {
	private:
		Punto posicion;
		float intensidad;
	public:
		Luz() { posicion = Punto(); intensidad = 0.0; };
		Luz(Punto p, float i) {
			posicion = p;
			intensidad = i;
		};
		//	Calcula la direccion en la que se ubica la luz segun el punto p
		Punto calcularDireccion(Punto p) {
			return Punto(	posicion.x - p.x,
							posicion.y - p.y,
							posicion.z - p.z
						);
		}
		//	retorna false si ese punto esta en sombra para esta luz
		//	despues cuando la hagamos mejor retorna el valor de la luz despues de pasar por los
		//	objetos transparentes
		bool determinarIluminacion(Punto p) {
			Punto direccion = p - posicion;
			bool hayIluminacion = true;
			float modulo = direccion.modulo();
			for (int i = 0; i < mundo.shapes.size(); ++i) {
				Punto* puntoResultado = NULL;
				int cantPuntos = mundo.shapes[i]->colisionaCon(posicion, p, puntoResultado);
				for (int cant = 0; cant < cantPuntos; cant++) {
					// Por cada colisión, quedarse con el punto más cercano descubierto hasta ahora
					// Ver si el punto está del lado adecuado
					Punto segmento = puntoResultado[cant] - posicion;
					if (segmento.productoInterno(direccion) >= 0) {
						// Si lo está, ver si su módulo es menor al del anterior punto más cercano
						if (segmento.modulo() < modulo) {
							hayIluminacion = false;
							//tengo que hacer esto porque con un break normal no alcanza
							goto finLoop;
						}
					}
				}
			}
		finLoop:
			return hayIluminacion;
		}
};

#endif
