#include "Shape.h"

#include "Utils.h"
#include "Punto.h"
#include "Color.h"
#include "Luz.h"

using namespace std;


Shape::Shape() {
	//TODO cargar esto a la hora de construir el objeto
	reflexion = refraccion = transparencia = constanteEspecular = 0.0;
	colorAmbiente = Color(100, 0, 0);
	colorDifuso = Color(100, 0, 0);
	colorEspecular = Color(100, 0, 0);
	constanteEspecular = 1.0;
}

Shape::Shape(float refle, float refra, float transp, Color amb, Color dif, Color esp, float constEsp) {
	reflexion = refle;
	refraccion = refra;
	transparencia = transp;
	colorAmbiente = amb;
	colorDifuso = dif;
	colorEspecular = esp;
	constanteEspecular = constEsp;
}

ColorInt Shape::calcularColorLuz(Punto colision, Punto p1, Punto p2) {
	ColorInt colorInt = ColorInt();
	//calculo si la superficie tiene luz o sombra
	for (int i = 0; i < Mundo::inst()->luces.size(); ++i) {
		Luz luz = Mundo::inst()->luces[i];
		Punto direccionLuz = luz.calcularDireccion(colision);
		Punto normal = calcularNormal(colision);
		if (direccionLuz * normal > 0) {
			Intensidad luzIntens = luz.determinarIluminacion(colision, this);
			if ((luzIntens.r > 0) || (luzIntens.g > 0) || (luzIntens.b > 0)) {
				//factor de atenuacion
				float factorAtt = min((1 / (constAtt + linearAtt * direccionLuz.modulo() + quadAtt * direccionLuz.modulo() * direccionLuz.modulo())), 1);
				//factor del angulo
				direccionLuz = direccionLuz.normalizar();
				float factorDif = direccionLuz * normal;
				//el vector R y su producto por V como aparece en las diapositivas
				Punto vectV = (p1 - colision).normalizar(); //las diapositivas no dicen pero me imagino que hay que normalizarlo
				Punto vectR = normal.productoEscalar(2 * (normal * direccionLuz)) - direccionLuz;
				float factorSpecRVnK = constanteEspecular * pow(vectR * vectV, nPhong);

				colorInt.red += truncar_0(luzIntens.r * factorAtt * (factorDif * colorDifuso.red + factorSpecRVnK * colorEspecular.red));
				colorInt.green += truncar_0(luzIntens.g * factorAtt * (factorDif * colorDifuso.green + factorSpecRVnK * colorEspecular.green));
				colorInt.blue += truncar_0(luzIntens.b * factorAtt * (factorDif * colorDifuso.blue + factorSpecRVnK * colorEspecular.blue));
			}
		}
		colorInt.red += colorAmbiente.red * luz.getAmb().r;
		colorInt.green += colorAmbiente.green * luz.getAmb().g;
		colorInt.blue += colorAmbiente.blue * luz.getAmb().b;
	}
	return colorInt;
}

ColorInt Shape::calcularColorReflexion(Punto colision, Punto p1, Punto p2, int profundidad) {
	//hay pila de cosas que ya estaban calculadas para la luz que las calculo de nuevo
	//podria ser una forma de optimizar quizas
	Punto normal = calcularNormal(colision);
	Punto direccionFuente = (p1 - colision).normalizar();
	//esto deberia simetrizar direccionFuente respecto a normal
	Punto direccionReflejo = normal.productoEscalar(2 * (normal * direccionFuente)) - direccionFuente;
	//esto me deberia dar un punto en la direccion de direccionReflejo
	Punto p2Refle = direccionReflejo + colision;
	Punto * resultado = NULL;
	int indiceMasCercano;
	Shape * shapeResultado = NULL;
	int cantidadPuntos = Shape::trace(colision, p2Refle, direccionReflejo, resultado, indiceMasCercano, shapeResultado, this);
	if (cantidadPuntos == 0) {
		return ColorInt(Mundo::inst()->background.red,
						Mundo::inst()->background.green,
						Mundo::inst()->background.blue
						);
	}
	else {
		return shapeResultado->calcularColor(resultado[indiceMasCercano], colision, p2Refle, profundidad);
	}
}


//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
//	Determina el color en el punto 'colision' para el rayo ->p1p2
ColorInt Shape::calcularColor(Punto colision, Punto p1, Punto p2, int profundidad) {
		//trabajo con int y despues los trunco
		//calculo si la superficie tiene luz o sombra
		ColorInt lightComponent = calcularColorLuz(colision, p1, p2);

		ColorInt refleComponent = ColorInt();
		ColorInt refraComponent = ColorInt();
		if (profundidad > 0) {
			//calculo el reflejo
			if (constanteEspecular > 0.0f) {
				refleComponent = calcularColorReflexion(colision, p1, p2, profundidad -1);
			}
		}

		return ColorInt(	/*(1 - reflexion) * */lightComponent.red + constanteEspecular * refleComponent.red,
							/*(1 - reflexion) * */lightComponent.green + constanteEspecular * refleComponent.green, 
							/*(1 - reflexion) * */lightComponent.blue + constanteEspecular * refleComponent.blue
							);

}

ColorInt Shape::calcularColorRefra(Punto colision, Punto p1, Punto p2, int profundidad) {
	ColorInt retorno(refraccion*255,refraccion*255,refraccion*255);
	return retorno;
}

ColorInt Shape::calcularColorRefle(Punto colision, Punto p1, Punto p2, int profundidad) {
	ColorInt retorno(reflexion * 255, reflexion * 255, reflexion * 255);
	return retorno;
}

//quedo bastante complicado
int Shape::trace(Punto p1, Punto p2, Punto direccion, Punto* &resultado, int &indiceMasCercano, Shape* &shapeResultado, Shape* ignorar) {
	shapeResultado = NULL;
	indiceMasCercano = -1;
	//borro lo que me viene
	delete[] resultado;
	resultado = new Punto[2];
	int cantPuntosResultado = 0;

	Punto* resultadoActual = NULL;
	int cantPuntos = 0;
	float modulo = 0.0;
	bool primerPunto = true;
	for (int it = 0; it < Mundo::inst()->shapes.size(); ++it) {
		if ((ignorar != NULL) && (ignorar == Mundo::inst()->shapes[it]))
			continue;
		cantPuntos = Mundo::inst()->shapes[it]->colisionaCon(p1, p2, resultadoActual);
		for (int cant = 0; cant < cantPuntos; cant++) {
			// Por cada colisión, quedarse con el punto más cercano descubierto hasta ahora
			// Ver si el punto está del lado adecuado
			Punto segmento = resultadoActual[cant] - p1;
			if (segmento.productoInterno(direccion) >= 0) {
				// Si lo está, ver si su módulo es menor al del anterior punto más cercano
				if ((primerPunto) || (segmento.modulo() < modulo)) {
					modulo = segmento.modulo();
					primerPunto = false;
					//asigno las salidas
					shapeResultado = Mundo::inst()->shapes[it];
					indiceMasCercano = cant;
					cantPuntosResultado = cantPuntos;
					for (int i = 0; i < cantPuntosResultado; ++i)
						resultado[i] = resultadoActual[i];

				}
			}
		}
		if (cantPuntos > 0)
			delete[] resultadoActual;
	}
	//cambio el tamano del array
	if (cantPuntosResultado == 1) {
		Punto aux = resultado[0];
		delete[] resultado;
		resultado = new Punto[1];
		resultado[0] = aux;
	}
	else if (cantPuntosResultado == 0) {
		delete[] resultado;
		resultado = NULL;
	}
	return cantPuntosResultado;
}

float Shape::getTransparencia() {
	return transparencia;
}