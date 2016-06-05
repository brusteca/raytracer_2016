#include "Mundo.h"
#include "Luz.h"
#include "Shape.h"

Mundo* Mundo::instance;

void Mundo::crearInstance() {
	instance = new Mundo;
}
Mundo* Mundo::inst() {
	return instance;
}