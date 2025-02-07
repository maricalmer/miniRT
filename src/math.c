#include "minirt.h"

int imin(int a, int b)
{
	if (a < b)
		return (a);
	else
		return (b);
}

void normalize(float vector[3])
{
	float temp;
	float magnitude;

	magnitude = sqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
	temp = 1 / magnitude;
	vector[0] *= temp;
	vector[1] *= temp;
	vector[2] *= temp;
}

float	dot_13_13(float a[3], float b[3])
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
}
