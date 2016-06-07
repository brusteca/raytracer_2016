#pragma once
#ifndef UTILS_H
#define UTILS_H

#include "Color.h"
#include <math.h>

using namespace std;

enum cantRaices {
	Dos, Una, Cero
};

struct raices {
	float a;
	float b;
	cantRaices cantidad;
};

raices bhaskara(float a, float b, float c);

float absFloat(float a);

float min(float a, float b);

//dado un int lo trunca a byte
//si es mayor a 255 devuelve 255
int truncar(int trunc);

//dado un colorInt lo trunca a Color
Color truncar(ColorInt trunc);

#endif

