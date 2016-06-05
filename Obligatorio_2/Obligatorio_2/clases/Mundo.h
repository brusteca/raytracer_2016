#pragma once
#ifndef MUNDO_H
#define MUNDO_H

//#include "Luz.h"
#include "Shape.h"
#include "Color.h"
#include <vector>

class Luz;

struct Mundo {
	vector<Luz> luces;
	vector<Shape*> shapes;
	Color background;
} mundo;

#endif
