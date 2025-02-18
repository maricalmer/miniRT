#include "../include/minirt.h"

float	dot_13_13(float a[3], float b[3])
{
	return (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]);
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

void	calculate_refraction_ray(float *p, float n[3], float v[3], float r_idx)
{
	float	vp[3];
	float	v_dot_n_inv;
	int		i;
	float	kf;

	v_dot_n_inv = 1 / (-dot_13_13(v, n));
	i = -1;
	while (++i < 3)
		vp[i] = v[i] * v_dot_n_inv;
	kf = r_idx * r_idx * (vp[0] * vp[0] + vp[1] * vp[1] + vp[2] * vp[2]);
	kf -= (vp[0] + n[0]) * (vp[0] + n[0]) + (vp[1] + n[1]) * (vp[1] + n[1]) + (vp[2] + n[2]) * (vp[2] + n[2]);
	kf = sqrtf(kf);
	kf = 1 / kf;
	i = -1;
	while (++i < 3)
		p[i] = kf * (n[i] + vp[i]) - n[i];
	normalize(p);
}
int	main(void)
{
	float n[] = {0, 1, 0};
	float v[] = {0.7071, -0.7071, 0};
	float idx = 1.5;

	float res[3];

	calculate_refraction_ray(res, n, v, idx);
	printf("%.4f\t%.4f\t%.4f\n", res[0], res[1], res[2]);
	v[0] = -1;
	v[1] = -1;
	v[2] = 0;
	normalize(v);
	calculate_refraction_ray(res, n, v, idx);
	printf("%.4f\t%.4f\t%.4f\n", res[0], res[1], res[2]);
	calculate_refraction_ray(res, n, v, 1.1);
	printf("%.4f\t%.4f\t%.4f\n", res[0], res[1], res[2]);
}