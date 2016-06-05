#include "Shape.h"

#include "Utils.h"
#include "Punto.h"
#include "Color.h"
#include "Luz.h"

using namespace std;

Shape::Shape() {
	reflexion = refraccion = 0.0;
	colorAmbiente.red = 100;
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
				
				direccionLuz = direccionLuz.normalizar();
				float factorDif = direccionLuz * normal;
				color.red += luz.getDif().r * colorAmbiente.red * factorDif;
				color.green += luz.getDif().g * colorAmbiente.green * factorDif;
				color.blue += luz.getDif().b * colorAmbiente.blue * factorDif;
			}
			color.red += colorAmbiente.red * luz.getAmb().r;
			color.green += colorAmbiente.green * luz.getAmb().g;
			color.blue += colorAmbiente.blue * luz.getAmb().b;
		}
		return color;
	}
}