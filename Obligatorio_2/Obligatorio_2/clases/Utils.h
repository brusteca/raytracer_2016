#pragma once
#ifndef UTILS_H
#define UTILS_H

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

#endif

