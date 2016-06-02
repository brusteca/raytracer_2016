#include "pugixml-1.7/src/pugixml.hpp"

#include "clases\Cilindro.h"
#include "clases\Esfera.h"
#include "clases\Poligono.h"
#include "clases\Utils.h"
#include "clases\Shape.h"

using namespace std;

int main() {
	// Leer archivo xml y construir shapes
	string directorio;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		if (nodo->name() == "Esfera") {
		
		}
		else if (nodo->name() == "Cilindro") {
		
		}
		else if (nodo->name() == "Poligono") {
		
		}
	}
}