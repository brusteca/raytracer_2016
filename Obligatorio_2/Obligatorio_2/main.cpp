#include "pugixml-1.7/src/pugixml.hpp"

using namespace std;

int main() {
	// Leer archivo xml y construir shapes
	string directorio;
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		
	}
}