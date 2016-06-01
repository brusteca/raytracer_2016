#define byte unsigned char


#include "pugixml-1.7/src/pugixml.hpp"

using namespace std;



int main() {
	// Leer archivo xml y construir shapes
	string directorio;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		
	}

	int width, height;
	width = height = 100;
	byte * red_value = new byte[width*height];
	byte * green_value = new byte[width*height];
	byte * blue_value = new byte[width*height];

	for (int y = 0; y<height; y++)
		for (int x = 0; x<width; x++)
		{
			red_value[y*width + height] = 0;
			green_value[y*width + height] = 255;
			blue_value[y*width + height] = 0;
		}

	//imprimir imagen
	FILE *f;
	errno_t err = fopen_s(&f, "out.ppm", "w+");;
	fprintf(f, "P6\n%i %i 255\n", width, height);
	for (int y = 0; y<height; y++)
		for (int x = 0; x<width; x++)
		{
			fputc(red_value[y*width + height], f);   // 0 .. 255
			fputc(green_value[y*width + height], f); // 0 .. 255
			fputc(blue_value[y*width + height], f);  // 0 .. 255
		}
	fclose(f);


}