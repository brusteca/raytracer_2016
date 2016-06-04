#pragma once
#ifndef MUNDO_H
#define MUNDO_H

#include "Luz.h"
#include "Shape.h"
#include "Color.h"
#include <vector>

struct Mundo {
	vector<Luz> luces;
	vector<Shape*> shapes;
	Color background;
};

Mundo mundo;

#endif
