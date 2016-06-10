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

//Dado un int si es menor a 0 retorna 0
int truncar_0(int trunc);

//dado un colorInt lo trunca a Color
Color truncar(ColorInt trunc);

//dado un color lo transforma en ColorInt
ColorInt colorToInt(Color c);

//retorna un float entre [min,max]
//no garantizado que max y min esten incluidos
float randFloat(float min, float max);

#endif

