#define BPP 24			//3 8bit rgb values

#include <iostream>

#define OUTPUT_FOLDER "historial"
#include <sstream>
#include <iomanip>
#include <time.h>

#include "pugixml-1.7/src/pugixml.hpp"
#include "FreeImage.h"

#include "clases\Cilindro.h"
#include "clases\Esfera.h"
#include "clases\Poligono.h"
#include "clases\Utils.h"
#include "clases\Color.h"
#include "clases\Mundo.h"
#include "clases\Punto.h"
#include "clases\Shape.h"

using namespace std;

void guardarImagen(int width, int height, byte* red_value, byte* green_value, byte* blue_value) {
	FreeImage_Initialise();
	FIBITMAP * bitmap = FreeImage_Allocate(width, height, BPP);
	RGBQUAD color;
	if (!bitmap)
		perror("No se genero el bitmap");
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			color.rgbRed = red_value[y*width + x];
			color.rgbGreen = green_value[y*width + x];
			color.rgbBlue = blue_value[y*width + x];
			FreeImage_SetPixelColor(bitmap, x, y, &color);

		}
	stringstream ss;
	auto t = time(nullptr);
	tm tm;
	localtime_s(&tm, &t);
	ss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
	string filename = "historial/output" + ss.str() + ".png";

	if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str()))
		cout << "Imagen guardada exitosamente!" << endl;

	FreeImage_DeInitialise();
}


int main(int argc, char** argv) {
	// Leer archivo xml y construir shapes
	string directorio = "mundo/mundo.xml";
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	Punto posicionCamara = Punto();
	Punto infIzq, supDer; // Atributos de Ventana
	int width, height;
	width = height = 0;
	float profundidadVentana = 0.0;
	Color backgroundColor;
	backgroundColor.blue = backgroundColor.green = backgroundColor.red = 0;
	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		string nombre = string(nodo->name());
		if (nombre == "Esfera") {
			Punto centro(stof(nodo->attribute("x").value()), stof(nodo->attribute("y").value()), stof(nodo->attribute("z").value()));
			float radio = stof(nodo->attribute("radio").value());
			mundo.shapes.push_back(new Esfera(centro,radio));
		}
		else if (nombre == "Cilindro") {

		}
		else if (nombre == "Poligono") {

		}
		else if (nombre == "Luz") {

		}
		else if (nombre == "Background") {

		}
		else if (nombre == "PuntosVentana") {
			profundidadVentana = stof(nodo->attribute("prof").value());
			infIzq = Punto(stof(nodo->attribute("xI").value()), stof(nodo->attribute("yI").value()),profundidadVentana);
			supDer = Punto(stof(nodo->attribute("xD").value()), stof(nodo->attribute("yD").value()), profundidadVentana);
		}
		else if (nombre == "Resolucion") {
			height = stoi(nodo->attribute("height").value());
			width = stoi(nodo->attribute("width").value());
		}				
		else if (nombre == "PosicionCamara") {
			posicionCamara = Punto(stof(nodo->attribute("x").value()), stof(nodo->attribute("y").value()), stof(nodo->attribute("z").value()));
		}
	}
	// Control de heigh y width, si son menores a 2, abortar
	if ((height < 2) || (width < 2)) {
		cout << "Error: la resolución no es suficiente " << endl;
		return 1;
	}
	// Matriz de colores
	Color* matriz = new Color[height*width];

	Punto pixel;
	Shape* shapeElegido = NULL;
	// Variables precalculables
	Punto direccion = (infIzq - posicionCamara); // El producto interno de este vector con cualquier punto no debería dar negativo.
	// Loop de píxeles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Establecer pixel actual
			pixel = Punto(infIzq.x + (supDer.x - infIzq.x)*j/(width-1),infIzq.y + (supDer.y - infIzq.y)*i/(height -1) ,profundidadVentana);
			// Para cada objeto, obtener sus puntos de contacto con el rayo camara-pixel
			Punto puntoMasCercano = Punto();			
			Punto* puntoResultado = NULL;
			int cantPuntos = 0;
			float modulo = 0.0;
			bool primerPunto = true;
			for (vector<Shape*>::iterator it = mundo.shapes.begin(); it != mundo.shapes.end(); it++) {
				cantPuntos = (*it)->colisionaCon(posicionCamara, pixel, puntoResultado);
				for (int cant = 0; cant < cantPuntos; cant++) {
					// Por cada colisión, quedarse con el punto más cercano descubierto hasta ahora
					// Ver si el punto está del lado adecuado
					Punto segmento = puntoResultado[cant] - posicionCamara;
					if (segmento.productoInterno(direccion) >= 0) {
						// Si lo está, ver si su módulo es menor al del anterior punto más cercano
						if ((primerPunto)||(segmento.modulo() < modulo)) {
							puntoMasCercano = puntoResultado[cant];
							modulo = segmento.modulo();
							primerPunto = false;
							shapeElegido = (*it);
						}						
					}
				}
				if (primerPunto)
					matriz[i*width + j] = backgroundColor;
				else
					// Color del Shape elegido
					matriz[i*width + j] = shapeElegido->calcularColor(puntoMasCercano,posicionCamara,pixel);
				if (cantPuntos > 0)
					delete[] puntoResultado;
			}
		}
	}

	//generar imagen
	//width = height = 100; // Comentado para utilizar resolución

	byte * red_value = new byte[width*height];
	byte * green_value = new byte[width*height];
	byte * blue_value = new byte[width*height];

	for (int y = 0; y<height; y++)
		for (int x = 0; x<width; x++)
		{
			red_value[y*width + x] = matriz[y*width + x].red; // = 0
			green_value[y*width + x] = matriz[y*width + x].green; // = 255
			blue_value[y*width + x] = matriz[y*width + x].blue; // = 0
		}

	//imprimir imagen
	guardarImagen(width, height, red_value, green_value, blue_value);

}