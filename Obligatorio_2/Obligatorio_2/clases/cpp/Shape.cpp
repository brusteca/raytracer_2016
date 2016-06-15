#include "Shape.h"

#include "Utils.h"
#include "Punto.h"
#include "Color.h"
#include "Luz.h"

#include <math.h>

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

ColorInt Shape::calcularColorReflexion(Punto colision, Punto p1, Punto p2, int profundidad, float material) {
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
		return shapeResultado->calcularColor(resultado[indiceMasCercano], colision, p2Refle, profundidad, material);
	}
}

//limitacion: no puede haber una cosa transparente adentro de otra
//para solucionar esto hay que transformar material en un stack
ColorInt Shape::calcularColorRefraccion(Punto colision, Punto p1, Punto p2, int profundidad, float material) {
	//para calcular el vector de salida:
	//		-transformo el vector de incidencia en uno que sale del punto colision (direccionFuente)
	//		-calculo el cross product entre direccionFuente y la normal para hallar el vector perpendicular
	//		-calculo el angulo entre el direccionFuente y la normal
	//		-calculo el angulo de salida usando los materiales
	//		-roto usando la matriz de rotacion respecto a ese angulo y el eje es el del cross product
	//		-si todo sale bien me sale un vector para el lado correcto
	Punto normal = calcularNormal(colision);
	Punto direccionFuente = (p1 - colision).normalizar();
	//el orden de los operadores lo pense pero podria estar mal
	Punto eje = normal.productoVectorial(direccionFuente).normalizar();
	//espero que me de el angulo 'chico'
	float anguloEntrada = direccionFuente.angulo(normal);
	//logica de adentro/afuera: si el rayo viene con el mismo material es que estoy adentro y por lo tanto tengo que salir afuera
	//esto implica que no puede haber un objeto refractante dentro de otro
	float materialOpuesto;
	if (this->refraccion != material) {
		materialOpuesto = this->refraccion;
	} else {
		materialOpuesto = Mundo::inst()->refraccionAire;
	}

	//TODO considerar la reflexion interna total
	float anguloSalida = asin( (sin(anguloEntrada) * (material / materialOpuesto)) );

	Punto vectorSalida = normal.negado().rotar(eje, anguloSalida);
	//esto es para que no colisione con la superficie de salida
	Punto colision_noError = colision + vectorSalida.productoEscalar(0.0001f);
	Punto p2Refrac = vectorSalida + colision_noError;

	Punto * resultado = NULL;
	int indiceMasCercano;
	Shape * shapeResultado = NULL;
	// paso null en el shape de origen para que pueda colisionar consigo mismo
	int cantidadPuntos = Shape::trace(colision_noError, p2Refrac, vectorSalida, resultado, indiceMasCercano, shapeResultado, NULL);
	if (cantidadPuntos == 0) {
		return ColorInt(Mundo::inst()->background.red,
			Mundo::inst()->background.green,
			Mundo::inst()->background.blue
			);
	}
	else {
		return shapeResultado->calcularColor(resultado[indiceMasCercano], colision, p2Refrac, profundidad, materialOpuesto);
	}


}

//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
//	Determina el color en el punto 'colision' para el rayo ->p1p2
ColorInt Shape::calcularColor(Punto colision, Punto p1, Punto p2, int profundidad, float material) {
		//trabajo con int y despues los trunco
		//calculo si la superficie tiene luz o sombra
		ColorInt lightComponent = calcularColorLuz(colision, p1, p2);

		ColorInt refleComponent = ColorInt();
		ColorInt refraComponent = ColorInt();
		if (profundidad > 0) {
			//calculo el reflejo
			//reflexion la uso ahora para determinar si una superficie es reflectante o no
			//con esto puedo hacer superficies espejadas o superficies que son simplemente bruñidas
			if ((constanteEspecular > 0.0f) && (reflexion > 0.0f)){
				refleComponent = calcularColorReflexion(colision, p1, p2, profundidad -1, material);
			}
			//calculo la refraccion
			if (transparencia > 0.0f) {
				refraComponent = calcularColorRefraccion(colision, p1, p2, profundidad - 1, material);
			}
		}

		return ColorInt(	(1 - transparencia) * (lightComponent.red + constanteEspecular * refleComponent.red ) + transparencia * refraComponent.red,
							(1 - transparencia) * (lightComponent.green + constanteEspecular * refleComponent.green ) + transparencia * refraComponent.green,
							(1 - transparencia) * (lightComponent.blue + constanteEspecular * refleComponent.blue ) + transparencia * refraComponent.blue
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