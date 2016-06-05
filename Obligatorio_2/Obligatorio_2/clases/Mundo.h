#pragma once
#ifndef MUNDO_H
#define MUNDO_H

//#include "Luz.h"
//#include "Shape.h"
#include "Color.h"
#include <vector>

class Luz;
class Shape;

//singleton sin lazy initialization
class Mundo {
private:
	static Mundo* instance;
public:
	static void crearInstance();
	static Mundo* inst();

	vector<Luz> luces;
	vector<Shape*> shapes;
	Color background;
};

#endif
