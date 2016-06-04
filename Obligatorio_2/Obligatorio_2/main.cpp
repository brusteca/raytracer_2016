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


int main() {
	// Leer archivo xml y construir shapes
	string directorio;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	Punto posicionCamara = Punto();
	Punto infIzq, supDer; // Atributos de Ventana
	int width, height;
	width = height = 0;
	float profundidadVentana = 0.0;
	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		if (nodo->name() == "Esfera") {

		}
		else if (nodo->name() == "Cilindro") {

		}
		else if (nodo->name() == "Poligono") {

		}
		else if (nodo->name() == "Luz") {

		}
		else if (nodo->name() == "Background") {

		}
		else if (nodo->name() == "PuntosVentana") {

		}
		else if (nodo->name() == "Width") {

		}
		else if (nodo->name() == "Height") {

		}
		else if (nodo->name() == "ProfundidadVentana") {

		}
	}

	Punto pixel;
	// Loop de píxeles
	for (int i = 0; i < height; i++) {
		for (int j = 0; i < width; j++) {
			// Establecer pixel actual
			pixel = Punto(infIzq.x + (supDer.x - infIzq.x)*j/(width-1),infIzq.y + (supDer.y - infIzq.y)*i/(height -1) ,profundidadVentana);
			// Para cada objeto, obtener sus puntos de contacto con el rayo camara-pixel

		}
	}

	//generar imagen
	int width, height;
	width = height = 100;

	byte * red_value = new byte[width*height];
	byte * green_value = new byte[width*height];
	byte * blue_value = new byte[width*height];

	for (int y = 0; y<height; y++)
		for (int x = 0; x<width; x++)
		{
			red_value[y*width + x] = 0;
			green_value[y*width + x] = 255;
			blue_value[y*width + x] = 0;
		}

	for (int y = 0; y < height/2; y++) {
		red_value[y * width] = 255;
	}

	//imprimir imagen
	guardarImagen(width, height, red_value, green_value, blue_value);

}