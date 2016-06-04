#define byte unsigned char
#define BPP 24			//3 8bit rgb values

#include <iostream>

#define OUTPUT_FOLDER "historial"
#include <sstream>
#include <iomanip>
#include <time.h>

#include "pugixml-1.7/src/pugixml.hpp"
#include "FreeImage.h"

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

	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		
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