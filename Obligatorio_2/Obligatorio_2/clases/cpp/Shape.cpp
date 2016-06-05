#include "Shape.h"

#include "Utils.h"
#include "Punto.h"
#include "Color.h"
#include "Luz.h"

using namespace std;

Shape::Shape() {
	reflexion = refraccion = 0.0;
	colorAmbiente.red = 255;
	colorAmbiente.green = 0;
	colorAmbiente.blue = 0;
}

Shape::Shape(float refle, float refra) : Shape() {
	reflexion = refle;
	refraccion = refra;
}

//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
//	Determina el color en el punto 'colision' para el rayo ->p1p2
Color Shape::calcularColor(Punto colision, Punto p1, Punto p2) {
	{
		Color color;
		color.red = color.green = color.blue = 0;
		//calculo si la superficie tiene luz o sombra
		for (int i = 0; i < Mundo::inst()->luces.size(); ++i) {
			Luz luz = Mundo::inst()->luces[i];
			Punto direccionLuz = luz.calcularDireccion(colision);
			Punto normal = calcularNormal(colision);
			if ((direccionLuz * normal > 0) &&
				luz.determinarIluminacion(colision, this)) {

				color.red += colorAmbiente.red;
				color.green += colorAmbiente.green;
				color.blue += colorAmbiente.blue;
			}
		}
		return color;
	}
}