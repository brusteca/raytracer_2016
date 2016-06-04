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

raices bhaskara(float a, float b, float c) {
	raices retorno;
	if (b*b - 4 * a*c == 0.0) {
		retorno.cantidad = Una;
		retorno.a = b*b / (2 * a);
		retorno.b = 0.0;
	}
	else if ((b*b - 4 * a*c < 0.0)||(a == 0.0)) {
		retorno.cantidad = Cero;
		retorno.a = retorno.b = 0.0;
	}
	else {
		retorno.cantidad = Dos;
		retorno.a = (-b + sqrt(b*b - 4 * a*c))/(2*a);
		retorno.b = (-b - sqrt(b*b - 4 * a*c))/(2*a);
	}
	return retorno;
};

float absFloat(float a) {
	if (a < 0)
		return -a;
	else
		return a;
}


#endif

