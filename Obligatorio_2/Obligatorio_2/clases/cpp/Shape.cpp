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

//	Retorna NULL si no hay intersección, si la hay retorna el punto con menor z positivo.
//	Determina el color en el punto 'colision' para el rayo ->p1p2
Color Shape::calcularColor(Punto colision, Punto p1, Punto p2) {
	{
		//trabajo con int y despues los trunco
		int colorRed, colorGreen, colorBlue;
		colorRed = colorGreen = colorBlue = 0;
		//calculo si la superficie tiene luz o sombra
		for (int i = 0; i < Mundo::inst()->luces.size(); ++i) {
			Luz luz = Mundo::inst()->luces[i];
			Punto direccionLuz = luz.calcularDireccion(colision);
			Punto normal = calcularNormal(colision);
			if ((direccionLuz * normal > 0) &&
				luz.determinarIluminacion(colision, this)) {
				//factor de atenuacion
				float factorAtt = min((1 / (constAtt + linearAtt * direccionLuz.modulo + quadAtt * direccionLuz.modulo * direccionLuz.modulo)), 1);
				//factor del angulo
				direccionLuz = direccionLuz.normalizar();
				float factorDif = direccionLuz * normal;
				//el vector R y su producto por V como aparece en las diapositivas
				Punto vectV = (p1 - colision).normalizar(); //las diapositivas no dicen pero me imagino que hay que normalizarlo
				Punto vectR = normal.productoEscalar(2 * (normal * direccionLuz)) - direccionLuz;
				float factorSpecRVnK = constanteEspecular * pow(vectR * vectV, nPhong);

				colorRed += truncar( luz.getDif().r * factorAtt * (factorDif * colorDifuso.red + factorSpecRVnK * colorEspecular.red) );
				colorGreen += truncar( luz.getDif().g * factorAtt * (factorDif * colorDifuso.green + factorSpecRVnK * colorEspecular.green) );
				colorBlue += truncar( luz.getDif().b * factorAtt * (factorDif * colorDifuso.blue + factorSpecRVnK * colorEspecular.blue) );
			}
			colorRed += colorAmbiente.red * luz.getAmb().r;
			colorGreen += colorAmbiente.green * luz.getAmb().g;
			colorBlue += colorAmbiente.blue * luz.getAmb().b;
		}
		return Color(truncar(colorRed), truncar(colorGreen), truncar(colorBlue) );
	}
}