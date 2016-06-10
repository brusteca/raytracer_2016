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
#include "clases\Prisma.h"

#include "clases\Mundo.h"

using namespace std;

void guardarImagen(int width, int height, Color* colores) {
	FreeImage_Initialise();
	FIBITMAP * bitmap = FreeImage_Allocate(width, height, BPP);
	RGBQUAD color;
	if (!bitmap)
		perror("No se genero el bitmap");
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x)
		{
			color.rgbRed = colores[y*width + x].red;
			color.rgbGreen = colores[y*width + x].green;
			color.rgbBlue = colores[y*width + x].blue;
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
	Mundo::crearInstance();
	// Leer archivo xml y construir shapes
	//string directorio = "mundo/mundo_prisma.xml";
	string directorio = string(argv[1]);
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(directorio.c_str());

	Punto posicionCamara = Punto();
	Punto infIzq, supDer; // Atributos de Ventana
	int width, height;
	width = height = 0;
	float profundidadVentana = 0.0;
	Mundo::inst()->background = Color(0,0,0);
	for (pugi::xml_node_iterator nodo = doc.begin(); nodo != doc.end(); nodo++) {
		string nombre = string(nodo->name());
		if (nombre == "Shape") {
			//cargo los datos del shape
			float refle = stof(nodo->attribute("refle").value());
			float refra = stof(nodo->attribute("refra").value());
			float transp = stof(nodo->attribute("transp").value());
			Color amb = Color(	stoi(nodo->attribute("Ramb").value()),
								stoi(nodo->attribute("Gamb").value()),
								stoi(nodo->attribute("Bamb").value())
								);
			Color dif = Color(	stoi(nodo->attribute("Rdif").value()),
								stoi(nodo->attribute("Gdif").value()),
								stoi(nodo->attribute("Bdif").value())
								);
			Color esp = Color(	stoi(nodo->attribute("Resp").value()),
								stoi(nodo->attribute("Gesp").value()),
								stoi(nodo->attribute("Besp").value())
								);
			float constEsp = stof(nodo->attribute("constEsp").value());
			//cargo los datos del tipo especifico
			string tipo = string(nodo->attribute("tipo").value());
			if (tipo == "Esfera") {
				Punto centro(stof(nodo->attribute("x").value()),
					stof(nodo->attribute("y").value()),
					stof(nodo->attribute("z").value())
					);
				float radio = stof(nodo->attribute("radio").value());
				Mundo::inst()->shapes.push_back(new Esfera(centro, radio, refle, refra, transp, amb, dif, esp, constEsp));
			}
			else if (tipo == "Cilindro") {
				// Crear cilindro
				Punto centro(stof(nodo->attribute("x").value()),
					stof(nodo->attribute("y").value()),
					stof(nodo->attribute("z").value())
					);
				float altura = stof(nodo->attribute("altura").value());
				float radio = stof(nodo->attribute("radio").value());
				Mundo::inst()->shapes.push_back(new Cilindro(radio, centro, altura, refle, refra, transp, amb, dif, esp, constEsp));
			}
			else if (tipo == "Poligono") {
				vector<Punto> puntosPoligono;
				for (pugi::xml_node_iterator puntos = nodo->begin(); puntos != nodo->end(); puntos++) {
					if (string(puntos->name()) == "Vertice") {
						// Crear cada punto e ingresarlo en el vector
						Punto ingreso = Punto(stof(puntos->attribute("x").value()),
							stof(puntos->attribute("y").value()),
							stof(puntos->attribute("z").value())
							);
						puntosPoligono.push_back(ingreso);
					}
				}
				Shape* poligono = new Poligono(puntosPoligono, refle, refra, transp, amb, dif, esp, constEsp);
				Mundo::inst()->shapes.push_back(poligono);
			}
			else if (tipo == "Prisma") {
				vector<Punto> vertices;
				for (pugi::xml_node_iterator puntos = nodo->begin(); puntos != nodo->end(); puntos++) {
					if (string(puntos->name()) == "Vertice") {
						// Crear cada punto e ingresarlo en el vector
						Punto ingreso = Punto(stof(puntos->attribute("x").value()),
							stof(puntos->attribute("y").value()),
							stof(puntos->attribute("z").value())
							);
						vertices.push_back(ingreso);
					}
				}
				float alt = stof(nodo->attribute("altura").value());
				Mundo::inst()->shapes.push_back(new Prisma(vertices, alt, refle, refra, transp, amb, dif, esp, constEsp));
			}
		} 
		else if (nombre == "Luz") {
			Luz luz(Punto(	stof(nodo->attribute("x").value()), 
							stof(nodo->attribute("y").value()), 
							stof(nodo->attribute("z").value())
							), 
					Intensidad(	stof(nodo->attribute("Ramb").value()),
							stof(nodo->attribute("Gamb").value()),
							stof(nodo->attribute("Bamb").value())
							),
					Intensidad(	stof(nodo->attribute("Rdif").value()),
							stof(nodo->attribute("Gdif").value()),
							stof(nodo->attribute("Bdif").value())
							),
					Intensidad(	stof(nodo->attribute("Resp").value()),
							stof(nodo->attribute("Gesp").value()),
							stof(nodo->attribute("Besp").value())
							)
					);
			Mundo::inst()->luces.push_back(luz);
		}
		else if (nombre == "Background") {
			// Obtener colores de fondo
			Mundo::inst()->background = Color(	stoi(nodo->attribute("red").value()),
												stoi(nodo->attribute("green").value()),
												stoi(nodo->attribute("blue").value())
												);
		}
		else if (nombre == "PuntosVentana") {
			profundidadVentana = stof(nodo->attribute("prof").value());
			infIzq = Punto(	stof(nodo->attribute("xI").value()), 
							stof(nodo->attribute("yI").value()),
							profundidadVentana
							);
			supDer = Punto(	stof(nodo->attribute("xD").value()), 
							stof(nodo->attribute("yD").value()), 
							profundidadVentana
							);
		}
		else if (nombre == "Resolucion") {
			height = stoi(nodo->attribute("height").value());
			width = stoi(nodo->attribute("width").value());
		}				
		else if (nombre == "PosicionCamara") {
			posicionCamara = Punto(	stof(nodo->attribute("x").value()), 
									stof(nodo->attribute("y").value()), 
									stof(nodo->attribute("z").value())
									);
		}
	}
	// Control de height y width, si son menores a 2, abortar
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
	// Loop de píxeles (raytracing)
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Establecer pixel actual
			pixel = Punto(	infIzq.x + (supDer.x - infIzq.x)*j/(width-1),
							infIzq.y + (supDer.y - infIzq.y)*i/(height -1),
							profundidadVentana
							);
			// Para cada objeto, obtener sus puntos de contacto con el rayo camara-pixel		
			Punto* puntoResultado = NULL;
			int indiceResultado = -1;
			int cantPuntos = Shape::trace(posicionCamara, pixel, direccion, puntoResultado, indiceResultado, shapeElegido);
			if (cantPuntos == 0)
				matriz[i*width + j] = Mundo::inst()->background;
			else {
				// Color del Shape elegido
				matriz[i*width + j] = truncar(shapeElegido->calcularColor(puntoResultado[indiceResultado], posicionCamara, pixel, 10));
				delete[] puntoResultado;
			}
		}
	}

	//imprimir imagen
	guardarImagen(width, height, matriz);

}