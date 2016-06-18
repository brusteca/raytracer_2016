#define BPP 24			//3 8bit rgb values

#include <iostream>

#define OUTPUT_FOLDER "historial"
#include <sstream>
#include <iomanip>
#include <time.h>
#include <thread>

#include <cstdlib>

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

void guardarImagen(int width, int height, Color* colores, string app = "") {
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
	string filename = "historial/output" + ss.str() + app + ".png";

	if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str()))
		cout << "Imagen guardada exitosamente!" << endl;

	FreeImage_DeInitialise();
}

//obtiene los puntos con el algoritmo jittered
void JitteredAA(Punto pixel, float ladoXpixel, float ladoYpixel, Punto * puntosAA) {
	puntosAA[0] = Punto(pixel.x + randFloat(0.0f, ladoXpixel / 2.0f),
		pixel.y + randFloat(0.0f, ladoYpixel / 2.0f),
		pixel.z
		);
	puntosAA[1] = Punto(pixel.x + randFloat(0.0f, ladoXpixel / 2.0f),
		pixel.y - randFloat(0.0f, ladoYpixel / 2.0f),
		pixel.z
		);
	puntosAA[2] = Punto(pixel.x - randFloat(0.0f, ladoXpixel / 2.0f),
		pixel.y - randFloat(0.0f, ladoYpixel / 2.0f),
		pixel.z
		);
	puntosAA[3] = Punto(pixel.x - randFloat(0.0f, ladoXpixel / 2.0f),
		pixel.y + randFloat(0.0f, ladoYpixel / 2.0f),
		pixel.z
		);
}
//obtiene los puntos usando grid
void GridAA(Punto pixel, float ladoXpixel, float ladoYpixel, Punto * puntosAA) {
	puntosAA[0] = Punto(pixel.x + ladoXpixel / 4.0f,
		pixel.y + ladoYpixel / 4.0f,
		pixel.z
		);
	puntosAA[1] = Punto(pixel.x + ladoXpixel / 4.0f,
		pixel.y - ladoYpixel / 4.0f,
		pixel.z
		);
	puntosAA[2] = Punto(pixel.x - ladoXpixel / 4.0f,
		pixel.y - ladoYpixel / 4.0f,
		pixel.z
		);
	puntosAA[3] = Punto(pixel.x - ladoXpixel / 4.0f,
		pixel.y + ladoYpixel / 4.0f,
		pixel.z
		);
}

void calcularImagen(Color* matriz, Color* matrizRefle, Color* matrizRefra,
					Punto infIzq, Punto supDer, int width, int height, 
					Punto posicionCamara, float profundidadVentana,
					int iMin, int iMax, int jMin, int jMax) {
	Punto pixel;
	Shape* shapeElegido = NULL;
	// Variables precalculables
	Punto direccion = (infIzq - posicionCamara); // El producto interno de este vector con cualquier punto no debería dar negativo.
												 // Loop de píxeles (raytracing)
												 // Antialiasing
	srand(static_cast <unsigned> (time(0)));
	float ladoXpixel = (supDer.x - infIzq.x) / (width - 1);
	float ladoYpixel = (supDer.y - infIzq.y) / (height - 1);
	for (int i = iMin; i < iMax; i++) {
		for (int j = jMin; j < jMax; j++) {
			// Establecer pixel actual
			pixel = Punto(infIzq.x + ladoXpixel * j,
				infIzq.y + ladoYpixel * i,
				profundidadVentana
			);
			//defino los 4 puntos que voy a usar en AA
			Punto puntosAA[4];
			JitteredAA(pixel, ladoXpixel, ladoYpixel, puntosAA);
			ColorInt coloresAA[4];
			ColorInt coloresAARefle[4];
			ColorInt coloresAARefra[4];
			for (int pix = 0; pix < 4; ++pix) {
				// Para cada objeto, obtener sus puntos de contacto con el rayo camara-pixel		
				Punto* puntoResultado = NULL;
				int indiceResultado = -1;
				int cantPuntos = Shape::trace(posicionCamara, puntosAA[pix], direccion, puntoResultado, indiceResultado, shapeElegido);
				if (cantPuntos == 0) {
					coloresAA[pix] = colorToInt(Mundo::inst()->background);
					coloresAARefra[pix] = ColorInt(50, 50, 50);
					coloresAARefle[pix] = ColorInt(50, 50, 50);
				}
				else {
					// Color del Shape elegido
					coloresAA[pix] = shapeElegido->calcularColor(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					coloresAARefra[pix] = shapeElegido->calcularColorRefra(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					coloresAARefle[pix] = shapeElegido->calcularColorRefle(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					delete[] puntoResultado;
				}

			}
			matriz[i*width + j] = truncar(ColorInt(((coloresAA[0].red + coloresAA[1].red + coloresAA[2].red + coloresAA[3].red) / 4),
				((coloresAA[0].green + coloresAA[1].green + coloresAA[2].green + coloresAA[3].green) / 4),
				((coloresAA[0].blue + coloresAA[1].blue + coloresAA[2].blue + coloresAA[3].blue) / 4)
			)
			);
			matrizRefra[i*width + j] = truncar(ColorInt(((coloresAARefra[0].red + coloresAARefra[1].red + coloresAARefra[2].red + coloresAARefra[3].red) / 4),
				((coloresAARefra[0].green + coloresAARefra[1].green + coloresAARefra[2].green + coloresAARefra[3].green) / 4),
				((coloresAARefra[0].blue + coloresAARefra[1].blue + coloresAARefra[2].blue + coloresAARefra[3].blue) / 4)
			)
			);
			matrizRefle[i*width + j] = truncar(ColorInt(((coloresAARefle[0].red + coloresAARefle[1].red + coloresAARefle[2].red + coloresAARefle[3].red) / 4),
				((coloresAARefle[0].green + coloresAARefle[1].green + coloresAARefle[2].green + coloresAARefle[3].green) / 4),
				((coloresAARefle[0].blue + coloresAARefle[1].blue + coloresAARefle[2].blue + coloresAARefle[3].blue) / 4)
			)
			);
		}
	}
}

int main(int argc, char** argv) {
	Mundo::crearInstance();

	// Cargar parámetros
	bool THREADING = true;
	bool AUXILIARES = false;
	for (int i = 0; i < argc; i++) {
		if (string(argv[i]) == "-nothread")
			THREADING = false;
		else if (string(argv[i]) == "-aux")
			AUXILIARES = true;
	}
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
	// Matrices de colores
	Color* matriz = new Color[height*width];
	Color* matrizRefle = new Color[height*width];
	Color* matrizRefra = new Color[height*width];

	/*void calcularImagen(Color* matriz, Color* matrizRefle, Color* matrizRefra,
					Punto infIzq, Punto supDer, int width, int height, 
					Punto posicionCamara, float profundidadVentana,
					int iMin, int iMax, int jMin, int jMax) {*/

	//Threading
	thread* first = NULL;
	thread* second = NULL;
	thread* third = NULL;
	thread* fourth = NULL;
	if (THREADING) {
		first = new thread(calcularImagen, matriz, matrizRefle, matrizRefra, infIzq, supDer,
			width, height, posicionCamara, profundidadVentana,
			0, height / 2, 0, width / 2);
		second = new thread (calcularImagen, matriz, matrizRefle, matrizRefra, infIzq, supDer,
			width, height, posicionCamara, profundidadVentana,
			height / 2, height, 0, width / 2);
		third = new thread(calcularImagen, matriz, matrizRefle, matrizRefra, infIzq, supDer,
			width, height, posicionCamara, profundidadVentana,
			0, height / 2, width / 2, width);
		fourth = new thread(calcularImagen, matriz, matrizRefle, matrizRefra, infIzq, supDer,
			width, height, posicionCamara, profundidadVentana,
			height / 2, height, width / 2, width);
	}
	else
		first = new thread(calcularImagen, matriz, matrizRefle, matrizRefra, infIzq, supDer,
			width, height, posicionCamara, profundidadVentana,
			0, height, 0, width);
	/*
	Punto pixel;
	Shape* shapeElegido = NULL;
	// Variables precalculables
	Punto direccion = (infIzq - posicionCamara); // El producto interno de este vector con cualquier punto no debería dar negativo.
	// Loop de píxeles (raytracing)
	// Antialiasing
	srand(static_cast <unsigned> (time(0)));
	float ladoXpixel = (supDer.x - infIzq.x) / (width - 1);
	float ladoYpixel = (supDer.y - infIzq.y) / (height - 1);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Establecer pixel actual
			pixel = Punto(	infIzq.x + ladoXpixel * j,
							infIzq.y + ladoYpixel * i,
							profundidadVentana
							);
			//defino los 4 puntos que voy a usar en AA
			Punto puntosAA[4];
			JitteredAA(pixel, ladoXpixel, ladoYpixel, puntosAA);
			ColorInt coloresAA[4];
			ColorInt coloresAARefle[4];
			ColorInt coloresAARefra[4];
			for (int pix = 0; pix < 4; ++pix) {
				// Para cada objeto, obtener sus puntos de contacto con el rayo camara-pixel		
				Punto* puntoResultado = NULL;
				int indiceResultado = -1;
				int cantPuntos = Shape::trace(posicionCamara, puntosAA[pix], direccion, puntoResultado, indiceResultado, shapeElegido);
				if (cantPuntos == 0) {
					coloresAA[pix] = colorToInt(Mundo::inst()->background);
					coloresAARefra[pix] = ColorInt(50, 50, 50);
					coloresAARefle[pix] = ColorInt(50, 50, 50);
				}
				else {
					// Color del Shape elegido
					coloresAA[pix] = shapeElegido->calcularColor(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					coloresAARefra[pix] = shapeElegido->calcularColorRefra(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					coloresAARefle[pix] = shapeElegido->calcularColorRefle(puntoResultado[indiceResultado], posicionCamara, puntosAA[pix], 10);
					delete[] puntoResultado;
				}

			}
			matriz[i*width + j] = truncar(ColorInt(	((coloresAA[0].red + coloresAA[1].red + coloresAA[2].red + coloresAA[3].red )/4),
													((coloresAA[0].green + coloresAA[1].green + coloresAA[2].green + coloresAA[3].green ) / 4),
													((coloresAA[0].blue + coloresAA[1].blue + coloresAA[2].blue + coloresAA[3].blue ) / 4)
													)
											);
			matrizRefra[i*width + j] = truncar(ColorInt(((coloresAARefra[0].red + coloresAARefra[1].red + coloresAARefra[2].red + coloresAARefra[3].red) / 4),
				((coloresAARefra[0].green + coloresAARefra[1].green + coloresAARefra[2].green + coloresAARefra[3].green) / 4),
				((coloresAARefra[0].blue + coloresAARefra[1].blue + coloresAARefra[2].blue + coloresAARefra[3].blue) / 4)
				)
				);
			matrizRefle[i*width + j] = truncar(ColorInt(((coloresAARefle[0].red + coloresAARefle[1].red + coloresAARefle[2].red + coloresAARefle[3].red) / 4),
				((coloresAARefle[0].green + coloresAARefle[1].green + coloresAARefle[2].green + coloresAARefle[3].green) / 4),
				((coloresAARefle[0].blue + coloresAARefle[1].blue + coloresAARefle[2].blue + coloresAARefle[3].blue) / 4)
				)
				);
		}
	}*/
	first->join();
	if (THREADING) {
		second->join();
		third->join();
		fourth->join();
	}
	delete first;
	delete second;
	delete third;
	delete fourth;
	//imprimir imagen
	guardarImagen(width, height, matriz, "");
	if (AUXILIARES) {
		guardarImagen(width, height, matrizRefle, "_refle");
		guardarImagen(width, height, matrizRefra, "_refra");
	}

}