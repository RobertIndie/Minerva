#include "util.h"

bool
Equal(float a, float b, float accuracy)
{
	float t = a - b;
	t = t > 0 ? t : (-t);
	return t < accuracy;
}